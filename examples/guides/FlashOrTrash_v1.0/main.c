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
#include "sema.h"
#include "cond.h"
#include "mutex.h"
#include "tsrb.h"
#include "cmd.h"

#define SX126X_MSG_QUEUE        (8U)
#define SX126X_STACKSIZE        (THREAD_STACKSIZE_DEFAULT)
#define SX126X_MSG_TYPE_ISR     (0x3456)
#define SX126X_MAX_PAYLOAD_LEN  (128U)

#define PACKET_SIZE 8
#define QUEUE_SIZE 32 //must be a power of 2

static char stack[SX126X_STACKSIZE];
static kernel_pid_t _recv_pid;

static char effectStack[THREAD_STACKSIZE_DEFAULT];
static kernel_pid_t _effect_pid = NULL;

static uint8_t message[SX126X_MAX_PAYLOAD_LEN];

static sx126x_t sx1262;
static netdev_t *netdev = &sx1262.netdev;

static mutex_t lock;
static cond_t condition;
static uint16_t cmdIndex = 0;

static uint8_t crc8(const uint8_t *data, uint8_t len)
{
    uint8_t crc = 0x00;
    for (uint8_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t b = 0; b < 8; b++) {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x31; // polynomial x^8 + x^5 + x^4 + 1
            else
                crc <<= 1;
        }
    }
    return crc;
}

uint8_t * generatePacket(cmd_t message){
    static uint8_t packet[PACKET_SIZE];

    packet[0] = (message.cmdIndex >> 8) & 0xFF;   // high byte
    packet[1] = message.cmdIndex & 0xFF;          // low byte
    packet[2] = message.TTL;
    packet[3] = message.CMD;
    packet[4] = message.RED;
    packet[5] = message.GREEN;
    packet[6] = message.BLUE;

    // compute CRC over first 7 bytes
    packet[7] = crc8(packet, 7);

    return packet;
}

cmd_t unpackPacket(uint8_t* packet){
    cmd_t result = {0};

    // verify CRC
    uint8_t computed_crc = crc8(packet, PACKET_SIZE - 1);
    uint8_t received_crc = packet[PACKET_SIZE - 1];
    if (computed_crc != received_crc) {
        // CRC mismatch — return empty/invalid command
        return result;
    }

    // unpack bytes into struct
    result.valid = true;
    result.cmdIndex = (packet[0] << 8) | packet[1];
    result.TTL      = packet[2];
    result.CMD      = packet[3];
    result.RED      = packet[4];
    result.GREEN    = packet[5];
    result.BLUE     = packet[6];

    return result;
}

static tsrb_t rxCMDs;
static tsrb_t txCMDs;
static cmd_t rxCMDbuff[QUEUE_SIZE];
static cmd_t txCMDbuff[QUEUE_SIZE];

void init_thread_tools(void){
    tsrb_init(&rxCMDs, (uint8_t*)rxCMDbuff, sizeof(rxCMDbuff));
    tsrb_init(&txCMDs, (uint8_t*)txCMDbuff, sizeof(txCMDbuff));
    mutex_init(&lock);
    cond_init(&condition);
}

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

            cmd_t rxCMD = unpackPacket(message);
            if (rxCMD.valid){
                if (rxCMD.cmdIndex < cmdIndex){
                    cmdIndex = rxCMD.cmdIndex;
                    if (rxCMD.TTL-- > 0){
                        uint8_t* returnMessage = generatePacket(rxCMD);
                        iolist_t iolist = {
                            .iol_base = returnMessage,
                            .iol_len  = sizeof(returnMessage),
                            .iol_next = NULL,
                        };
                        netdev->driver->send(netdev, &iolist);
                        netopt_state_t state = NETOPT_STATE_RX;
                        netdev->driver->set(netdev, NETOPT_STATE, &state, sizeof(state));
                    }
                    mutex_lock(&lock);
                    tsrb_add(&rxCMDs, (uint8_t*)&rxCMD, sizeof(rxCMD));
                    mutex_unlock(&lock);
                    cond_signal(&condition);
                }
            }
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

static void delay(int seconds)
{
    ztimer_sleep(ZTIMER_USEC, seconds * US_PER_SEC);
}

int main(void)
{
    delay(2);
    ANT_SW_ON;
    puts("Init SX1262...");
    printf("sx1262 driver %p\n", netdev->driver);

    sx126x_setup(&sx1262, &sx126x_params[0], 0);

    printf("sx1262 driver %p\n", netdev->driver);

    puts("Init netdev...");
    netdev->event_callback = event_cb;
    int res = netdev->driver->init(netdev);
    printf("something went right %d\n", res);

    _recv_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                              0, _recv_thread, netdev,
                              "recv_thread");

    if (_recv_pid <= KERNEL_PID_UNDEF) {
        puts("Creation of receiver thread failed");
        return 1;
    }

    netopt_state_t state = NETOPT_STATE_RX;
    netdev->driver->set(netdev, NETOPT_STATE, &state, sizeof(state));
    puts("sleeping mcu");
    cortexm_sleep(1);

    while (1) {
        if (tsrb_empty(&rxCMDs)){
            cond_wait(&condition, &lock);
        }
        else{
            mutex_lock(&lock);
        }
        cmd_t message;
        tsrb_get(&rxCMDs, (uint8_t*)&message, sizeof(message));
        mutex_unlock(&lock);

        if (thread_getstatus(_effect_pid) != STATUS_STOPPED) {
            //somehow kill the thread...
        }

        _effect_pid = thread_create(effectStack, sizeof(effectStack), 
                            THREAD_PRIORITY_MAIN - 1,
                            0, effectFuncs[message.CMD], message,
                            "effect_thread");

    }

    return 0;
}
