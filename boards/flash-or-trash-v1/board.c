/**
 * @ingroup     boards_flash-or-trash-v1
 * @{
 *
 * @file
 * @brief       Board specific configuration for the flash-or-trash-v1.0 board
 *
 * @author      Timothy Szabo <a1852299@adelaide.edu.au>
 */

// Include general cpu and board headers
#include "cpu.h"
#include "board.h"

//include gpio headers to enable gpio functions
#include "periph/gpio.h"

void board_init(void)
{
    /* setup GPIO */
    // Sets parameters for NRF52 port 1 gpio
    NRF_P1->DIRSET = (GPIO_MASK_P1);
    NRF_P0->DIRSET = (GPIO_MASK_P0);
    // Initialises GPIO pins to off
    NRF_P1->OUTCLR = (GPIO_MASK_P1);
    NRF_P0->OUTCLR = (GPIO_MASK_P0);

    // Initialises the DIO2 pin as an input
    gpio_init(DIO2_PIN, GPIO_IN);
}
