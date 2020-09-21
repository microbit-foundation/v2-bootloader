/**
 * Copyright (c) 2016 - 2019, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/** @file
 *
 * @defgroup bootloader_secure_ble main.c
 * @{
 * @ingroup dfu_bootloader_api
 * @brief Bootloader project main file for secure DFU.
 *
 */

#include <stdint.h>
#include "nrf_mbr.h"
#include "nrf_bootloader.h"
#include "nrf_bootloader_app_start.h"
#include "nrf_bootloader_dfu_timers.h"
#include "nrf_dfu.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "app_error.h"
#include "app_error_weak.h"
#include "nrf_bootloader_info.h"
#include "nrf_delay.h"


#include "microbit_sdk.h"
extern int nRF5SDK_mods_dfu_percent_complete();


static void on_error(void)
{
    NRF_LOG_FINAL_FLUSH();

#if NRF_MODULE_ENABLED(NRF_LOG_BACKEND_RTT)
    // To allow the buffer to be flushed by the host.
    nrf_delay_ms(100);
#endif
#ifdef NRF_DFU_DEBUG_VERSION
    NRF_BREAKPOINT_COND;
#endif
    NVIC_SystemReset();
}


void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
    NRF_LOG_ERROR("%s:%d", p_file_name, line_num);
    on_error();
}


void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info)
{
    NRF_LOG_ERROR("Received a fault! id: 0x%08x, pc: 0x%08x, info: 0x%08x", id, pc, info);
    on_error();
}


void app_error_handler_bare(uint32_t error_code)
{
    NRF_LOG_ERROR("Received an error: 0x%08x!", error_code);
    on_error();
}


#define microbit_timer_SECOND 200

static uint32_t microbit_timer_tick = 0;

static bool microbit_timer_display = false;

#define microbit_progress_PERIOD 60

static bool     microbit_progress_started = false;
static uint32_t microbit_progress_tick;


static void microbit_timer_callback(void)
{
    microbit_timer_tick++;
    
    if ( microbit_timer_display)
        microbit_display_update();
}

static void microbit_timer_start(void)
{
    nrf_bootloader_user_timer_start( NRF_BOOTLOADER_MS_TO_TICKS(5), microbit_timer_callback);
}

static inline void microbit_timer_stop(void)
{
    nrf_bootloader_user_timer_stop();
}


static inline void microbit_timer_display_stop(void)
{
    microbit_timer_display = false;
}

static inline void microbit_timer_display_start(void)
{
    if ( !microbit_timer_display)
    {
      microbit_progress_started = false;
      microbit_display_start();
      microbit_timer_display = true;
      microbit_timer_start();
    }
}


static void microbit_dfu_symbol( const uint8_t *symbol)
{
    microbit_display_symbol( symbol);
    microbit_timer_display_start();
}


static void microbit_dfu_start(void)
{
    microbit_timer_display_stop();
    microbit_progress_start();
    microbit_timer_start();
    microbit_progress_tick = microbit_timer_tick;
    microbit_progress_started = true;
}


static inline void microbit_dfu_progress( int percent)
{
    if ( microbit_progress_started)
    {
        bool toggle = microbit_timer_tick - microbit_progress_tick >= microbit_progress_PERIOD;
        microbit_progress_next( percent, toggle);
        if ( toggle)
            microbit_progress_tick = microbit_timer_tick;
    }
    else
    {
        microbit_dfu_start();
    }
}


/**
 * @brief Function notifies certain events in DFU process.
 */
static void dfu_observer(nrf_dfu_evt_type_t evt_type)
{
    switch ( (int) evt_type)
    {
        case NRF_DFU_EVT_DFU_ABORTED:
            // nrf_bootloader's dfu_observer does not pass this event on before resetting
        case NRF_DFU_EVT_DFU_FAILED:
            microbit_dfu_symbol( microbit_symbol_cross);
            break;
        case NRF_DFU_EVT_DFU_INITIALIZED:
            // Occurs before softdevice is enabled
            break;
        case NRF_DFU_EVT_TRANSPORT_ACTIVATED:
            // Client has connected
            microbit_dfu_symbol( microbit_symbol_ble);
            break;
        case NRF_DFU_EVT_TRANSPORT_DEACTIVATED:
            // Client has disconnected
            microbit_dfu_symbol( microbit_symbol_plus);
            break;
        case NRF_DFU_EVT_DFU_STARTED:
            // Not called when restarting a DFU
            microbit_dfu_start();
            break;
        case NRF_DFU_EVT_DFU_ABORTED + 2:
            // from ble_dfu_transport_init when entered DFU mode and started advertising
            microbit_dfu_symbol( microbit_symbol_plus);
            break;
        case NRF_DFU_EVT_DFU_ABORTED + 1:
            // from on_data_obj_write_request
            microbit_dfu_progress( nRF5SDK_mods_dfu_percent_complete());
            break;
        default:
            break;
    }
}


/**@brief Function for application main entry. */
int main(void)
{
    uint32_t ret_val;

    // Must happen before flash protection is applied, since it edits a protected page.
    nrf_bootloader_mbr_addrs_populate();

// nRF5SDK_mods
// Remove all write protection
//    // Protect MBR and bootloader code from being overwritten.
//    ret_val = nrf_bootloader_flash_protect(0, MBR_SIZE, false);
//    APP_ERROR_CHECK(ret_val);
//    ret_val = nrf_bootloader_flash_protect(BOOTLOADER_START_ADDR, BOOTLOADER_SIZE, false);
//    APP_ERROR_CHECK(ret_val);

    (void) NRF_LOG_INIT(nrf_bootloader_dfu_timer_counter_get);
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    NRF_LOG_INFO("Inside main");

//    microbit_dfu_symbol( microbit_symbol_plus);
//    nrf_delay_ms(500);
//    microbit_dfu_symbol( microbit_symbol_cross);
//    nrf_delay_ms(500);
//    microbit_dfu_symbol( microbit_symbol_ble);
//    nrf_delay_ms(500);
//    microbit_dfu_start();
//    for ( int i = 0; i <= 400; i++)
//    {
//        microbit_dfu_progress( i / 4);
//        nrf_delay_ms( 20 + 10 * ( i % 4));
//    }
//    microbit_dfu_symbol( microbit_symbol_plus);
//    nrf_delay_ms(1000);
//    microbit_display_clear();
//    nrf_delay_ms(500);
//    microbit_timer_stop();

    ret_val = nrf_bootloader_init(dfu_observer);
    APP_ERROR_CHECK(ret_val);

    NRF_LOG_FLUSH();

    NRF_LOG_ERROR("After main, should never be reached.");
    NRF_LOG_FLUSH();

    APP_ERROR_CHECK_BOOL(false);
}

/**
 * @}
 */
