/**
 * @ingroup     boards_rak4631
 * @{
 *
 * @file
 * @brief       Board specific configuration for the RAK 4631 board
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
    // Initialises GPIO pins to off
    NRF_P1->OUTCLR = (GPIO_MASK_P1);

    // Initialises the DIO2 pin as an input
    gpio_init(DIO2_PIN, GPIO_IN);
}
