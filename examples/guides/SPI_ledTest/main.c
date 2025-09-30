/*
 * SPDX-FileCopyrightText: 2025 Tom Hert <git@annsann.eu>
 * SPDX-FileCopyrightText: 2025 HAW Hamburg
 * SPDX-License-Identifier: LGPL-2.1-only
 */

/*
 * For many printing related things, such as the puts function here
 * we import stdio, depending on your board, platform or form of output
 * it then includes the right definitions without the need to
 * worry about the specific details.
 */
#include <stdio.h>

/*
 * This is the main function of the program.
 * It serves as the entry point for the program and gets called once your CPU is
 * initialized.
 *
 * The function returns an integer value, which is the exit status
 * of the program. A return value of 0 indicates that the program has finished
 * successfully.
 */

#include "clk.h"
#include "board.h"
#include "periph_conf.h"
#include "timex.h"
#include "ztimer.h"
#include "WS2812_SPI.h"


static void delay(void)
{
    if (IS_USED(MODULE_ZTIMER)) {
        puts("ZTimer working");
        ztimer_sleep(ZTIMER_USEC, 2 * US_PER_SEC);
    }
    else {
        /*
         * As fallback for freshly ported boards with no timer drivers written
         * yet, we just use the CPU to delay execution and assume that roughly
         * 20 CPU cycles are spend per loop iteration.
         *
         * Note that the volatile qualifier disables compiler optimizations for
         * all accesses to the counter variable. Without volatile, modern
         * compilers would detect that the loop is only wasting CPU cycles and
         * optimize it out - but here the wasting of CPU cycles is desired.
         */
        puts("Using clock count");
        uint32_t loops = coreclk() / 20;
        for (volatile uint32_t i = 0; i < loops; i++) { }
    }
}

int main(void)
{
    bool tmp = false;
    while (1) {
        delay();
        if (tmp){
            setLED(0, 128, 0, 128);
            WS2812_Send();
        }
        else{
            setLED(0, 0, 0, 0);
            WS2812_Send();
        }
        tmp = !tmp;
    }

    return 0;
}
