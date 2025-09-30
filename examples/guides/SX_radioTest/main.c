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
#include "net/netdev.h"
#include "sx126x_netdev.h"
#include "periph/gpio.h"
#include "sx126x_params.h"

static sx126x_t sx1262;
static netdev_t *netdev = &sx1262.netdev;


/* event callback: called on TX done, RX done, etc. */
// static void event_cb(netdev_t *dev, netdev_event_t event)
// {
//     (void)dev;
//     if (event == NETDEV_EVENT_TX_COMPLETE) {
//         puts("TX complete");
//     }
//     else if (event == NETDEV_EVENT_RX_COMPLETE) {
//         puts("RX complete");
//     }
//     else {
//         printf("other event: %d\n", event);
//     }
// }

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
    delay();
    delay();
    delay();
    ANT_SW_ON;
    puts("Init SX1262...");
    printf("sx1262 driver %p\n", netdev->driver);

    sx126x_setup(&sx1262, &sx126x_params[0], 0);

    printf("sx1262 driver %p\n", netdev->driver);

    puts("Init netdev...");
    int res = netdev->driver->init(netdev);
    printf("something went right %d", res);
    // netdev->event_callback = event_cb;

    // uint16_t chan = 15;
    // netdev->driver->set(netdev, NETOPT_CHANNEL, &chan, sizeof(chan));

    //static uint8_t buf[] = "hello world";
    // iolist_t iolist = {
    //     .iol_base = buf,
    //     .iol_len  = sizeof(buf),
    //     .iol_next = NULL,
    // };


    // if (sx126x_init(&sx1262) != SPI_OK) {
    //     puts("SX1262 init failed!");
    //     return 1;
    // }
    while (1) {
        delay();
        // int res = netdev->driver->send(netdev, &iolist);
        // printf("send() returned %d\n", res);
        printf("test\n"); 
    }

    return 0;
}
