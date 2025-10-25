#include "Effects.h"
#include "WS2812_SPI.h"
#include "timex.h"
#include "ztimer.h"
#include "syncTools.h"

#define SLEEP_INTERVAL 20

static uint8_t RED_BASE = 0;
static uint8_t GREEN_BASE = 0;
static uint8_t BLUE_BASE = 0;

static void resetLED(void){
    setOneLED(RED_BASE, GREEN_BASE, BLUE_BASE);
}

static void delay(int msecs)
{
    int slept = 0;
    while (slept < msecs){
        mutex_lock(&lock);
        if (newCMDFlag){
            mutex_unlock(&lock);
            return;
        }
        ztimer_sleep(ZTIMER_MSEC, SLEEP_INTERVAL);
        slept += SLEEP_INTERVAL;
    }
    return;
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
