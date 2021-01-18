/*
SPDX-License-Identifier: MIT
Copyright (c) 2020 Micro:bit Educational Foundation and contributors

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
#include "microbit_apptimer.h"

#include "app_timer.h"


APP_TIMER_DEF( apptimer);


uint32_t microbit_apptimer_create( microbit_apptimer_handler_t handler)
{
    return app_timer_create( &apptimer, APP_TIMER_MODE_REPEATED, handler);
}

uint32_t microbit_apptimer_start( uint32_t ms, void *context)
{
    return app_timer_start( apptimer, APP_TIMER_TICKS(ms), context);
}

uint32_t microbit_apptimer_stop()
{
    return app_timer_stop( apptimer);
}
