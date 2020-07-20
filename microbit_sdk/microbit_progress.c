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

static int m_progress_x;
static int m_progress_y;
static int m_progress_max;


static void microbit_progress_togglePixel( int x, int y)
{
    microbit_display_togglePixel( x + microbit_LEDMap.width / 2, y + microbit_LEDMap.height / 2);
}


void microbit_progress_start()
{
    m_progress_max = 1;
    m_progress_x = m_progress_y = 0;
    
    microbit_display_clear();
    microbit_progress_togglePixel( 0, 0);
}


void microbit_progress_next( int percent)
{
    int pixels = microbit_LEDMap.width * microbit_LEDMap.height;
    int imax = pixels * percent / 100;
    
    microbit_display_togglePixel( 0, 0);
    
    while ( m_progress_max < imax)
    {
        //microbit_progress_togglePixel( m_progress_x, m_progress_y);
        
        m_progress_max++;
        
        if ( abs(m_progress_y) <= abs(m_progress_x) && ( m_progress_y != m_progress_x || m_progress_y <= 0))
            m_progress_y += m_progress_x <= 0 ? -1 : 1;
        else
            m_progress_x += m_progress_y <= 0 ? 1 : -1;
        
        microbit_progress_togglePixel( m_progress_x, m_progress_y);
    }
}
