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

#ifndef microbit_display_H
#define microbit_display_H

#include "microbit_config.h"


extern void microbit_display_start();

extern void microbit_display_updateRow();
extern void microbit_display_update();

extern void microbit_display_clear();

extern bool microbit_display_getPixel( int x, int y);
extern void microbit_display_setPixel( int x, int y);
extern void microbit_display_clearPixel( int x, int y);
extern void microbit_display_togglePixel( int x, int y);

extern void microbit_display_symbol( const uint8_t *symbol);

#endif // microbit_display_H
