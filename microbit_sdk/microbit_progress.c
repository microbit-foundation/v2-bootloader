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
#include "microbit_LEDMap.h"
#include "microbit_display.h"
#include "microbit_progress.h"

#include <stdlib.h>


//================================================================
#if microbit_config_PROGRESS_COLUMNS
//================================================================


void microbit_progress_start()
{
    microbit_LEDMap_configureHiDrive();
    
    for ( int col = 0; col < microbit_LEDMap.columns; col++)
        microbit_LEDMap_columnOn( col, false);

    for ( int row = 0; row < microbit_LEDMap.rows; row++)
        microbit_LEDMap_rowOn( row);
}


void microbit_progress_next( int percent)
{
    int imax = 1 + ( microbit_LEDMap.columns - 1) * percent / 100;
    
    for ( int col = 0; col < imax; col++)
        microbit_LEDMap_columnOn( col, true);
}

//================================================================
#else // microbit_config_PROGRESS_COLUMNS
//================================================================


static int m_progress_max;


static void microbit_progress_togglePixel( int x, int y)
{
    microbit_display_togglePixel( x + microbit_LEDMap.width / 2, y + microbit_LEDMap.height / 2);
}


static void microbit_progress_toggleSpiral( int idx)
{
    int i;
    int x = 0;
    int y = 0;
        
    for ( i = 0; i < idx; i++)
    {
        if ( abs(x) <= abs(y) && ( x != y || x <= 0))
            x += y <= 0 ? -1 : 1;
        else
            y += x <= 0 ? 1 : -1;
    }
    
    microbit_progress_togglePixel( x, y);
}


void microbit_progress_start()
{
    m_progress_max = 0;
    
    microbit_display_clear();
    microbit_progress_togglePixel( 0, 0);
}


void microbit_progress_next( int percent)
{
    int pixels = microbit_LEDMap.width * microbit_LEDMap.height;
    int imax = pixels * percent / 100;
    
    microbit_progress_togglePixel( 0, 0);
    
    while ( m_progress_max < imax)
    {
        m_progress_max++;
        microbit_progress_toggleSpiral( pixels - m_progress_max);
    }
}

//================================================================
#endif // microbit_config_PROGRESS_COLUMNS
//================================================================
