/**
 * board_microbit.c
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "board_microbit.h"


unsigned char board_microbit_led_row[5] = { 21, 22, 15, 24, 19 };
unsigned char board_microbit_led_col[5] = { 28, 11, 31, 37, 30 };


void board_microbit_init(void)
{
    // setup microbit LEDs
    for ( int i = 0; i < 5; i++)
    {
      nrf_gpio_cfg_output( board_microbit_led_row[i]);
      nrf_gpio_cfg_output( board_microbit_led_col[i]);
      nrf_gpio_pin_write( board_microbit_led_row[i], 0);
      nrf_gpio_pin_write( board_microbit_led_col[i], 1);
    }
 
    // set microbit LEDs column 0 to output
    // board_microbit.h uses the row numbers for the LEDs
    nrf_gpio_pin_write( board_microbit_led_col[ 0], 0);
}


#ifdef __cplusplus
}
#endif
