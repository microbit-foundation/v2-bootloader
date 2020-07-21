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
#include "microbit_font.h"
#include "microbit_ledmap.h"
#include "microbit_panic.h"

// Constants for microbit_panic

// length of message: face, E, code digit, code digit, code digit
#ifndef microbit_panic_MSGLEN
#define microbit_panic_MSGLEN         5
#endif

// position of first code digit
#ifndef microbit_panic_MSG1STDIGIT
#define microbit_panic_MSG1STDIGIT    2
#endif

// divisor for first digit
#ifndef microbit_panic_DIVMAX
#define microbit_panic_DIVMAX         100
#endif

// divisor base
#ifndef microbit_panic_DIVBASE
#define microbit_panic_DIVBASE        10
#endif

// Number of burn passes for each character
#ifndef microbit_panic_SCANS
#define microbit_panic_SCANS         40
#endif

// Passes left clear for repeated character
#ifndef microbit_panic_CLEARSCANS
#define microbit_panic_CLEARSCANS    3
#endif

// Delay cycles for each row
#ifndef microbit_panic_ROWDELAY
#define microbit_panic_ROWDELAY       (20000)
#endif


static const uint8_t panic_face[ MICROBIT_FONT_WIDTH] = { 0x1B, 0x1B, 0x0, 0x0E, 0x11};

static int panic_timeout = 0;


void microbit_panic_timeout(int iterations)
{
    panic_timeout = iterations;
}

void microbit_panic( int statusCode)
{
    uint8_t chr0 = 255;
    uint8_t chr;
    
    microbit_LEDMap_configure();
    
    for ( int repeat = 0; panic_timeout == 0 || repeat < panic_timeout; repeat++)
    {
        for ( int msgIdx = 0; msgIdx < microbit_panic_MSGLEN; msgIdx++, chr0 = chr)
        {
            // find the the current character and its font bytes
            chr = msgIdx ? 0x45 : 0;
            if ( msgIdx >= microbit_panic_MSG1STDIGIT)
            {
                // calculate divisor for this digit: 100s, 10s or units
                int div = microbit_panic_DIVMAX;
                for ( int digit = msgIdx - microbit_panic_MSG1STDIGIT; digit > 0; digit--)
                    div /= microbit_panic_DIVBASE;
                chr = 0x30 + ( statusCode / div) % microbit_panic_DIVBASE;
            }
            const uint8_t *fontBytes = chr ? microbit_font_char( chr) : panic_face;
            
            for ( int scan = 0; scan < microbit_panic_SCANS; scan++)
            {
                int rowOnCycles = microbit_panic_ROWDELAY;
                if ( chr == chr0 && scan < microbit_panic_CLEARSCANS)   // blank display for repeated characters
                    rowOnCycles = 0;
                else if ( repeat >= 5)                                  // dim display after 5 repetitions
                    rowOnCycles = rowOnCycles / 10;
                
                // turn each row on and off
                for ( int row = 0; row < microbit_LEDMap.rows; row++)
                {
                    for ( int col = 0; col < microbit_LEDMap.columns; col++)
                    {
                        microbit_LEDMap_columnOn( col,
                            fontBytes[ microbit_LEDMap.mapRCtoY( row, col)] & ( 1 << ( microbit_LEDMap.width - 1 - microbit_LEDMap.mapRCtoX( row, col))));
                    }
                        
                    if ( rowOnCycles)
                      microbit_LEDMap_rowOn( row);

                    for ( int i = 0; i < microbit_panic_ROWDELAY; i++)
                    {
                        nrf_gpio_pin_out_read( microbit_LEDMap.columnPins[0]);
                        if ( i == rowOnCycles - 1)
                            microbit_LEDMap_rowOff( row);
                    }
                }
            }
        }
    }
}
