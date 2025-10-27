#include "Effects.h"
#include "WS2812_SPI.h"
#include "timex.h"
#include "ztimer.h"
#include "syncTools.h"
#include <stdio.h>

#define SLEEP_INTERVAL 10

uint8_t RED_BASE = 0;
uint8_t GREEN_BASE = 0;
uint8_t BLUE_BASE = 0;

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
        mutex_unlock(&lock);
        ztimer_sleep(ZTIMER_USEC, SLEEP_INTERVAL*1000);
        slept += SLEEP_INTERVAL;
    }
    return;
}

static void fadeBetween(uint8_t fromR, uint8_t fromG, uint8_t fromB,
                        uint8_t toR, uint8_t toG, uint8_t toB,
                        int duration_ms)
{
    const int steps = duration_ms / LED_TIME_GUARD;
    float stepR = ((float)toR - fromR) / steps;
    float stepG = ((float)toG - fromG) / steps;
    float stepB = ((float)toB - fromB) / steps;

    float currR = fromR;
    float currG = fromG;
    float currB = fromB;

    for (int i = 0; i < steps; i++) {
        mutex_lock(&lock);
        if (newCMDFlag) {
            mutex_unlock(&lock);
            return;  // Interrupt fade if new command arrives
        }
        mutex_unlock(&lock);

        RED_BASE = (uint8_t)currR;
        GREEN_BASE = (uint8_t)currG;
        BLUE_BASE = (uint8_t)currB;
        setOneLED(RED_BASE, GREEN_BASE, BLUE_BASE);
        ztimer_sleep(ZTIMER_USEC, LED_TIME_GUARD * 1000);
        currR += stepR;
        currG += stepG;
        currB += stepB;
    }

    RED_BASE = toR;
    GREEN_BASE = toG;
    BLUE_BASE = toB;
    setOneLED(RED_BASE, GREEN_BASE, BLUE_BASE);
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

int effect_fade2s(cmd_t cmd)
{
    fadeBetween(RED_BASE, GREEN_BASE, BLUE_BASE,
                cmd.RED, cmd.GREEN, cmd.BLUE,
                2000);
    return 0;
}

int effect_fade5s(cmd_t cmd)
{
    fadeBetween(RED_BASE, GREEN_BASE, BLUE_BASE,
                cmd.RED, cmd.GREEN, cmd.BLUE,
                5000);
    return 0;
}

int effect_fade10s(cmd_t cmd)
{
    fadeBetween(RED_BASE, GREEN_BASE, BLUE_BASE,
                cmd.RED, cmd.GREEN, cmd.BLUE,
                10000);
    return 0;
}

int (*effectFuncs[])(cmd_t) = {
    effect_setColour,
    effect_blink500ms,
    effect_blink1000ms,
    effect_blink2000ms,
    effect_fade2s,
    effect_fade5s,
    effect_fade10s,
};
