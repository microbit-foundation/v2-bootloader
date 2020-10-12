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
#include "microbit_display.h"
#include "microbit_progress.h"

//#include <stdlib.h>

static int m_progress_max;


void microbit_progress_start()
{
    m_progress_max = 0;
    
    microbit_LEDMap_configureHiDrive();
    
    for ( int col = 0; col < microbit_LEDMap.columns; col++)
        microbit_LEDMap_columnOff( col);

    for ( int row = 0; row < microbit_LEDMap.rows; row++)
        microbit_LEDMap_rowOn( row);
    
    microbit_LEDMap_columnToggle( m_progress_max);
}


void microbit_progress_next( int percent, bool toggle)
{
    int cmax = microbit_LEDMap.columns;
    int pmax = cmax * percent / 100;
    
    while ( m_progress_max < pmax)
    {
        microbit_LEDMap_columnOn( m_progress_max, true);
        m_progress_max++;
    }
    
    if ( toggle && m_progress_max < cmax)
      microbit_LEDMap_columnToggle( m_progress_max);
}
