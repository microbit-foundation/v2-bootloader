/*
SPDX-License-Identifier: MIT License
Copyright (c) 2020, Micro:bit Educational Foundation and contributors

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

#ifndef microbit_apptimer_H
#define microbit_apptimer_H

/* To use the app_timer int nRF5SDK, compile these files
 *      $(SDK_ROOT)/components/libraries/timer/app_timer2.c \
 *      $(SDK_ROOT)/components/libraries/timer/drv_rtc.c \
 *      $(SDK_ROOT)/components/libraries/sortlist/nrf_sortlist.c \
 * with includes
 *      $(SDK_ROOT)/components/libraries/timer \
 *      $(SDK_ROOT)/components/libraries/sortlist \
 * and flags
 *      CFLAGS += -DAPP_TIMER_V2
 *      CFLAGS += -DAPP_TIMER_V2_RTC1_ENABLED
 *      ASMFLAGS += -DAPP_TIMER_V2
 *      ASMFLAGS += -DAPP_TIMER_V2_RTC1_ENABLED
*/

#include "microbit_config.h"


typedef void (* microbit_apptimer_handler_t)( void *context);

extern uint32_t microbit_apptimer_create( microbit_apptimer_handler_t handler);
extern uint32_t microbit_apptimer_start( uint32_t ms, void *context);
extern uint32_t microbit_apptimer_stop();

#endif // microbit_apptimer_H
