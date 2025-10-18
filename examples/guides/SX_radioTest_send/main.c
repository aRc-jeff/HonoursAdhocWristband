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
#include "net/lora.h"
#include "net/netdev.h"
#include "net/netdev/lora.h"
#include "sx126x.h"
#include "sx126x_netdev.h"
#include "periph/gpio.h"
#include "sx126x_params.h"

#define SX126X_MSG_QUEUE        (8U)
#define SX126X_STACKSIZE        (THREAD_STACKSIZE_DEFAULT)
#define SX126X_MSG_TYPE_ISR     (0x3456)
#define SX126X_MAX_PAYLOAD_LEN  (128U)

static char stack[SX126X_STACKSIZE];
static kernel_pid_t _recv_pid;

static char message[SX126X_MAX_PAYLOAD_LEN];

static sx126x_t sx1262;
static netdev_t *netdev = &sx1262.netdev;

static void event_cb(netdev_t *dev, netdev_event_t event)
{
    if (event == NETDEV_EVENT_ISR) {
        msg_t msg;
        msg.type = SX126X_MSG_TYPE_ISR;
        if (msg_send(&msg, _recv_pid) <= 0) {
            puts("sx126x_netdev: possibly lost interrupt.");
        }
    }
    else {
        switch (event) {
        case NETDEV_EVENT_RX_STARTED:
            puts("Data reception started");
            break;

        case NETDEV_EVENT_RX_COMPLETE:
        {
            size_t len = dev->driver->recv(dev, NULL, 0, 0);
            netdev_lora_rx_info_t packet_info;
            dev->driver->recv(dev, message, len, &packet_info);
            printf(
                "Received: \"%s\" (%" PRIuSIZE " bytes) - [RSSI: %i, SNR: %i, TOA: %" PRIu32 "ms]\n",
                message, len,
                packet_info.rssi, packet_info.snr,
                sx126x_get_lora_time_on_air_in_ms(&sx1262.pkt_params, &sx1262.mod_params)
                );
            netopt_state_t state = NETOPT_STATE_RX;
            dev->driver->set(dev, NETOPT_STATE, &state, sizeof(state));
            puts("sleeping mcu");
            cortexm_sleep(1);
        }
        break;

        case NETDEV_EVENT_TX_COMPLETE:
            puts("Transmission completed");
            break;

        case NETDEV_EVENT_TX_TIMEOUT:
            puts("Transmission timeout");
            break;

        default:
            printf("Unexpected netdev event received: %d\n", event);
            break;
        }
    }
}

void *_recv_thread(void *arg)
{
    netdev_t *netdev = arg;

    static msg_t _msg_queue[SX126X_MSG_QUEUE];

    msg_init_queue(_msg_queue, SX126X_MSG_QUEUE);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == SX126X_MSG_TYPE_ISR) {
            netdev->driver->isr(netdev);
        }
        else {
            puts("Unexpected msg type");
        }
    }
}

// /* event callback: called on TX done, RX done, etc. */
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
        ztimer_sleep(ZTIMER_USEC, 5 * US_PER_SEC);
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
    ANT_SW_ON;
    puts("Init SX1262...");
    printf("sx1262 driver %p\n", netdev->driver);

    sx126x_setup(&sx1262, &sx126x_params[0], 0);

    printf("sx1262 driver %p\n", netdev->driver);

    puts("Init netdev...");
    netdev->event_callback = event_cb;
    int res = netdev->driver->init(netdev);
    printf("something went right %d\n", res);

    static uint8_t buf[] = "hello world";
    iolist_t iolist = {
        .iol_base = buf,
        .iol_len  = sizeof(buf),
        .iol_next = NULL,
    };

    _recv_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                              0, _recv_thread, netdev,
                              "recv_thread");

    if (_recv_pid <= KERNEL_PID_UNDEF) {
        puts("Creation of receiver thread failed");
        return 1;
    }

    uint16_t chan = 15;
    netdev->driver->set(netdev, NETOPT_CHANNEL, &chan, sizeof(chan));
    uint32_t freq = 915000000;
    netdev->driver->set(netdev, NETOPT_CHANNEL_FREQUENCY, &freq, sizeof(freq));


    // netopt_state_t state = NETOPT_STATE_RX;
    // netdev->driver->set(netdev, NETOPT_STATE, &state, sizeof(state));
    // puts("sleeping mcu");
    // cortexm_sleep(1);

    while (1) {
        delay();
        int res = netdev->driver->send(netdev, &iolist);
        printf("send() returned %d\n", res);
    }

    return 0;
}
