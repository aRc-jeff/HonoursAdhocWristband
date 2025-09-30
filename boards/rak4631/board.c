/**
 * @ingroup     boards_rak4631
 * @{
 *
 * @file
 * @brief       Board specific configuration for the RAK 4631 board
 *
 * @author      Timothy Szabo <a1852299@adelaide.edu.au>
 */

#include "cpu.h"
#include "board.h"
#include "periph/gpio.h"

void board_init(void)
{
    /* setup GPIO */
    NRF_P1->DIRSET = (GPIO_MASK_P1);
    NRF_P1->OUTSET = (GPIO_MASK_P1);
    gpio_init(DIO2_PIN, GPIO_IN);
}
