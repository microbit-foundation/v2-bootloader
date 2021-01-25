/*
SPDX-License-Identifier: MIT License
Copyright (c) 2020 Micro:bit Educational Foundation

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "microbit_config.h"
#include "microbit_timer.h"
#include "microbit_display.h"

#include "nrf.h"
#include "nrf_timer.h"
#include "nrfx_timer.h"


nrfx_timer_t microbit_timer = NRFX_TIMER_INSTANCE(1);


static void microbit_timer_event_handler( nrf_timer_event_t event_type, void *p_context)
{
    switch ( event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:
            microbit_display_update();
            break;

        default:
            //Do nothing.
            break;
    }
}

uint32_t microbit_timer_init()
{
    nrfx_timer_config_t config =
    {
        .frequency          = NRF_TIMER_FREQ_1MHz,
        .mode               = NRF_TIMER_MODE_TIMER,
        .bit_width          = NRF_TIMER_BIT_WIDTH_16,
        .interrupt_priority = 2,
        .p_context          = NULL
    };
    
    uint32_t err = nrfx_timer_init( &microbit_timer, &config, microbit_timer_event_handler);
    if ( err != NRFX_SUCCESS)
        return err;

    nrfx_timer_extended_compare( &microbit_timer, NRF_TIMER_CC_CHANNEL0, 5000, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
    nrfx_timer_enable( &microbit_timer);
    
    return NRFX_SUCCESS;
}
