#include "Effects.h"
#include "WS2812_SPI.h"
#include "timex.h"
#include "ztimer.h"

static uint8_t RED_BASE = 0;
static uint8_t GREEN_BASE = 0;
static uint8_t BLUE_BASE = 0;

static void resetLED(){
    setOneLED(RED_BASE, GREEN_BASE, BLUE_BASE);
}

static void delay(int msecs)
{
    ztimer_sleep(ZTIMER_MSEC, msecs);
}

int effect_setColour(cmd_t cmd) {
    RED_BASE = cmd.RED;
    GREEN_BASE = cmd.GREEN;
    BLUE_BASE = cmd.BLUE;
    resetLED();
    return 0;
}

int effect_blink500ms(cmd_t cmd) {
    setOneLED(cmd.RED, cmd.GREEN, cmd.BLUE);
    delay(500);
    resetLED();
    return 0;
}

int effect_blink1000ms(cmd_t cmd) {
    setOneLED(cmd.RED, cmd.GREEN, cmd.BLUE);
    delay(1000);
    resetLED();
    return 0;
}

int effect_blink2000ms(cmd_t cmd) {
    setOneLED(cmd.RED, cmd.GREEN, cmd.BLUE);
    delay(2000);
    resetLED();
    return 0;
}


int (*effectFuncs[])(cmd_t) = {
    effect_setColour,
    effect_blink500ms,
    effect_blink1000ms,
    effect_blink2000ms,
};
