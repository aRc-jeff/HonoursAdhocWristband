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
#include "cmd.h"
#include "random.h"
#include "Effects.h"

#define BOARD_LOCATION_X 1
#define BOARD_LOCATION_Y 1

#define RADIO_SEND_DELAY 60

#define SX126X_MSG_QUEUE        (8U)
#define SX126X_STACKSIZE        (THREAD_STACKSIZE_DEFAULT)
#define SX126X_MSG_TYPE_ISR     (0x3456)
#define SX126X_MAX_PAYLOAD_LEN  (128U)
#define PACKET_SIZE 16 //must be a power of 2

static netopt_state_t RXstate = NETOPT_STATE_RX;
static char stack[SX126X_STACKSIZE];
static kernel_pid_t _recv_pid;

static uint8_t message[SX126X_MAX_PAYLOAD_LEN];

static sx126x_t sx1262;
static netdev_t *netdev = &sx1262.netdev;

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
    packet[7] = message.P1X;
    packet[8] = message.P1Y;
    packet[9] = message.P2X;
    packet[10] = message.P2Y;
    packet[11] = message.blank1;
    packet[12] = message.blank2;
    packet[13] = message.blank3;
    packet[14] = message.queueable;

    // compute CRC over first 15 bytes
    packet[15] = crc8(packet, PACKET_SIZE-1);

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

    result.P1X     = packet[7];
    result.P1Y     = packet[8];
    result.P2X     = packet[9];
    result.P2Y     = packet[10];
    result.blank1     = packet[11];
    result.blank2     = packet[12];
    result.blank3     = packet[13];
    result.queueable     = packet[14];

    return result;
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
        }
        break;

        case NETDEV_EVENT_TX_COMPLETE:
            puts("Transmission completed");
            netdev->driver->set(netdev, NETOPT_STATE, &RXstate, sizeof(RXstate));
            break;

        case NETDEV_EVENT_TX_TIMEOUT:
            puts("Transmission timeout");
            netdev->driver->set(netdev, NETOPT_STATE, &RXstate, sizeof(RXstate));
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

static void delay(int ms)
{
    ztimer_sleep(ZTIMER_USEC, ms * 1000);
}

void sendMessage(cmd_t message){
    uint8_t* returnMessage = generatePacket(message);
    iolist_t iolist = {
        .iol_base = returnMessage,
        .iol_len  = PACKET_SIZE,
        .iol_next = NULL,
    };
    netdev->driver->send(netdev, &iolist);
    delay(RADIO_SEND_DELAY);
}

uint8_t rand255(void) {
    return (uint8_t)(random_uint32() % 256);
}

int main(void)
{
    delay(2000);
    ANT_SW_ON;
    puts("Init SX1262...");
    sx126x_setup(&sx1262, &sx126x_params[0], 0);

    puts("Init netdev...");
    netdev->event_callback = event_cb;
    netdev->driver->init(netdev);

    uint32_t freq = 915000000;
    netdev->driver->set(netdev, NETOPT_CHANNEL_FREQUENCY, &freq, sizeof(freq));
    netdev->driver->set(netdev, NETOPT_STATE, &RXstate, sizeof(RXstate));

    puts("Create recv thread...");
    _recv_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                              0, _recv_thread, netdev,
                              "recv_thread");

    if (_recv_pid <= KERNEL_PID_UNDEF) {
        puts("Creation of receiver thread failed");
        return 1;
    }

    puts("Init Complete");

    cmd_t message;

    while (1) {

        // message = cmd_create(0, 255, 0, 0, 1, 1, 1, 1, false);
        // sendMessage(message);
        // delay(60);
        // message = cmd_create(0, 0, 255, 0, 1, 1, 1, 1, false);
        // sendMessage(message);
        // delay(60);
        // message = cmd_create(0, 0, 0, 255, 1, 1, 1, 1, false);
        // sendMessage(message);
        // delay(60);
        // message = cmd_create(0, 255, 0, 0, 1, 1, 1, 1, false);
        // sendMessage(message);
        // message = cmd_create(6, 0, 255, 0, 1, 1, 1, 1, false);
        // sendMessage(message);
        // delay(12000);
        // message = cmd_create(3, 0, 0, 255, 1, 1, 1, 1, false);
        // sendMessage(message);
        // delay(1);
        // message = cmd_create(3, 0, 0, 0, 1, 1, 1, 1, false);
        // sendMessage(message);
        // delay(1);
        // message = cmd_create(0, 255, 255, 255, 1, 1, 1, 1, false);
        // sendMessage(message);
        // delay(1);

        message = cmd_create(effect_id_t::SET_COLOUR, 255, 255, 255, 1, 1, 1, 1, false);
        sendMessage(message);
    }

    return 0;
}
