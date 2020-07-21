/*
MIT License

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
#include "microbit_ledmap.h"
#include "microbit_font.h"
#include "microbit_display.h"

#include <stdlib.h>
#include <string.h>

static uint8_t  displayImage[ microbit_LEDMap_HEIGHT];
static uint8_t  displayRow;


void microbit_display_start()
{
    microbit_LEDMap_configureHiDrive();
    displayRow = 0;
    microbit_display_updateRow();
}


void microbit_display_updateRow()
{
    for ( int col = 0; col < microbit_LEDMap.columns; col++)
    {
        microbit_LEDMap_columnOn( col, microbit_display_getPixel( microbit_LEDMap.mapRCtoX( displayRow, col), microbit_LEDMap.mapRCtoY( displayRow, col)));
    }

    microbit_LEDMap_rowOn( displayRow);
}


void microbit_display_update()
{
    microbit_LEDMap_rowOff( displayRow);
    
    displayRow++;
    if ( displayRow >= microbit_LEDMap.rows)
        displayRow = 0;
    
    microbit_display_updateRow();
}


void microbit_display_clear()
{
    memset( displayImage, 0, microbit_LEDMap.height);
}


bool microbit_display_getPixel( int x, int y)
{
    return displayImage[ y] & ( 1 << ( microbit_LEDMap.width - 1 - x)) ? true : false;
}


void microbit_display_setPixel( int x, int y)
{
    displayImage[ y] |= ( 1 << ( microbit_LEDMap.width - 1 - x));
}


void microbit_display_clearPixel( int x, int y)
{
    displayImage[ y] &= ~( 1 << ( microbit_LEDMap.width - 1 - x));
}


void microbit_display_togglePixel( int x, int y)
{
    displayImage[ y] ^= ( 1 << ( microbit_LEDMap.width - 1 - x));
}


void microbit_display_symbol( const uint8_t *symbol)
{
    memcpy( displayImage, symbol, microbit_LEDMap.height);
}
