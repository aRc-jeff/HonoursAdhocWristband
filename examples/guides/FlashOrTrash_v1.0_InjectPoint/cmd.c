#include "cmd.h"

uint16_t cmdIndex = 1;

cmd_t cmd_create(uint8_t CMD,
                 uint8_t RED, uint8_t GREEN, uint8_t BLUE,
                 uint8_t P1X, uint8_t P1Y, uint8_t P2X, uint8_t P2Y,
                 bool queueable) {
    cmd_t cmd = {
        .cmdIndex = cmdIndex++,
        .TTL = UNIVERSAL_TTL,
        .CMD = CMD,
        .RED = RED,
        .GREEN = GREEN,
        .BLUE = BLUE,
        .P1X = P1X,
        .P1Y = P1Y,
        .P2X = P2X,
        .P2Y = P2Y,
        .blank1 = 0,
        .blank2 = 0,
        .blank3 = 0,
        .queueable = queueable,
        .valid = true
    };
    return cmd;
}
