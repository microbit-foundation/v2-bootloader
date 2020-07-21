/*
The MIT License (MIT)

Copyright (c) 2016 Lancaster University, UK.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#ifndef microbit_ledmap_H
#define microbit_ledmap_H

#include "microbit_config.h"

#include "nrf.h"
#include "hal/nrf_gpio.h"


// LED matrix map for use by microbit_panic

typedef uint8_t microbit_LEDMapPinNumber;

typedef int (*microbit_LEDMapFnRCtoXY)( int row, int col);

typedef struct microbit_LEDMapStr
{
    int         width;                      // The physical width of the LED matrix, in pixels.
    int         height;                     // The physical height of the LED matrix, in pixels.
    int         rows;                       // The number of drive pins connected to LEDs.
    int         columns;                    // The number of sink pins connected to the LEDs.

    const microbit_LEDMapPinNumber *rowPins;      // Array of pin numbers to drive.
    const microbit_LEDMapPinNumber *columnPins;   // Array of pin numbers to sink.
    
    microbit_LEDMapFnRCtoXY mapRCtoX;       // Map logical LED positions to physical positions
    microbit_LEDMapFnRCtoXY mapRCtoY;
} microbit_LEDMapStr;


// LED matrix map values for micro:bit V2

// Width and height of LEDs
#ifndef microbit_LEDMap_WIDTH
#define microbit_LEDMap_WIDTH 5
#endif

#ifndef microbit_LEDMap_HEIGHT
#define microbit_LEDMap_HEIGHT 5
#endif

// Width and height of LED pins
#ifndef microbit_LEDMap_COLS
#define microbit_LEDMap_COLS 5
#endif

#ifndef microbit_LEDMap_ROWS
#define microbit_LEDMap_ROWS 5
#endif

// LED rows pinmap
#ifndef microbit_LEDMap_PINROW0
#define microbit_LEDMap_PINROW0 21
#endif

#ifndef microbit_LEDMap_PINROW1
#define microbit_LEDMap_PINROW1 22
#endif

#ifndef microbit_LEDMap_PINROW2
#define microbit_LEDMap_PINROW2 15
#endif

#ifndef microbit_LEDMap_PINROW3
#define microbit_LEDMap_PINROW3 24
#endif

#ifndef microbit_LEDMap_PINROW4
#define microbit_LEDMap_PINROW4 19
#endif

// LED columns pinmap
#ifndef microbit_LEDMap_PINCOL0
#define microbit_LEDMap_PINCOL0 28
#endif

#ifndef microbit_LEDMap_PINCOL1
#define microbit_LEDMap_PINCOL1 11
#endif

#ifndef microbit_LEDMap_PINCOL2
#define microbit_LEDMap_PINCOL2 31
#endif

#ifndef microbit_LEDMap_PINCOL3
#define microbit_LEDMap_PINCOL3 37
#endif

#ifndef microbit_LEDMap_PINCOL4
#define microbit_LEDMap_PINCOL4 30
#endif

static const microbit_LEDMapPinNumber microbit_LEDMap_rowPins[ microbit_LEDMap_ROWS] =
{
    microbit_LEDMap_PINROW0, microbit_LEDMap_PINROW1, microbit_LEDMap_PINROW2, microbit_LEDMap_PINROW3, microbit_LEDMap_PINROW4
};

static const microbit_LEDMapPinNumber microbit_LEDMap_columnPins[ microbit_LEDMap_COLS] =
{
    microbit_LEDMap_PINCOL0, microbit_LEDMap_PINCOL1, microbit_LEDMap_PINCOL2, microbit_LEDMap_PINCOL3, microbit_LEDMap_PINCOL4
};

static int microbit_LEDMap_RCtoX( int row, int col) { return col; }
static int microbit_LEDMap_RCtoY( int row, int col) { return row; }

static const microbit_LEDMapStr microbit_LEDMap =
{
    microbit_LEDMap_WIDTH, microbit_LEDMap_HEIGHT,
    microbit_LEDMap_COLS, microbit_LEDMap_ROWS,
    microbit_LEDMap_rowPins,
    microbit_LEDMap_columnPins,
    microbit_LEDMap_RCtoX,
    microbit_LEDMap_RCtoY
};


// Low level LEDs pin functions

static inline void microbit_LEDMap_rowOn(        int row)           { nrf_gpio_pin_set(   microbit_LEDMap.rowPins[ row]); }
static inline void microbit_LEDMap_rowOff(       int row)           { nrf_gpio_pin_clear( microbit_LEDMap.rowPins[ row]); }

static inline void microbit_LEDMap_columnOn(     int col, bool on)  { nrf_gpio_pin_write( microbit_LEDMap.columnPins[ col], on ? 0 : 1); }
static inline void microbit_LEDMap_columnOff(    int col)           { nrf_gpio_pin_set(   microbit_LEDMap.columnPins[ col]); }

static inline void microbit_LEDMap_pinOutput(    uint8_t pin)       { nrf_gpio_cfg_output( pin); }
static inline void microbit_LEDMap_pinOutputHiDrive( uint8_t pin)   { nrf_gpio_cfg( pin, NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT,
                                                                        NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE); }

static inline void microbit_LEDMap_configure()
{
    for ( int i = 0; i < microbit_LEDMap.columns; i++)  microbit_LEDMap_pinOutput( microbit_LEDMap.columnPins[i]);
    for ( int i = 0; i < microbit_LEDMap.rows;    i++)  microbit_LEDMap_pinOutput( microbit_LEDMap.rowPins[i]);
}

static inline void microbit_LEDMap_configureHiDrive()
{
    for ( int i = 0; i < microbit_LEDMap.columns; i++)  microbit_LEDMap_pinOutputHiDrive( microbit_LEDMap.columnPins[i]);
    for ( int i = 0; i < microbit_LEDMap.rows;    i++)  microbit_LEDMap_pinOutputHiDrive( microbit_LEDMap.rowPins[i]);
}

#endif
