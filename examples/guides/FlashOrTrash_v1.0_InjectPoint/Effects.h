/*
 * Effects.h
 *
 *  Created on: Oct 22, 2025
 *      Author: TIM
 */

#ifndef INC_EFFECTS_H_
#define INC_EFFECTS_H_

#define COLOUR_RED        255, 0, 0
#define COLOUR_GREEN      0, 255, 0
#define COLOUR_BLUE       0, 0, 255
#define COLOUR_YELLOW     255, 255, 0
#define COLOUR_CYAN       0, 255, 255
#define COLOUR_MAGENTA    255, 0, 255
#define COLOUR_ORANGE     255, 128, 0
#define COLOUR_PURPLE     128, 0, 255
#define COLOUR_WHITE      255, 255, 255
#define COLOUR_WARMWHITE  255, 220, 180
#define COLOUR_COOLWHITE  180, 220, 255
#define COLOUR_GRAY       128, 128, 128
#define COLOUR_PINK       255, 102, 178
#define COLOUR_LIME       191, 255, 0
#define COLOUR_SKYBLUE    135, 206, 235
#define COLOUR_TURQUOISE  64, 224, 208
#define COLOUR_LAVENDER   200, 150, 255
#define COLOUR_PEACH      255, 160, 120
#define COLOUR_MINT       170, 255, 195
#define COLOUR_CRIMSON    220, 20, 60
#define COLOUR_GOLD       255, 215, 0
#define COLOUR_TEAL       0, 128, 128
#define COLOUR_NAVY       0, 0, 128
#define COLOUR_MAROON     128, 0, 0
#define COLOUR_OFF	      0, 0, 0

typedef enum {
    SET_COLOUR,
    BLINK_500MS,
    BLINK_1000MS,
    BLINK_2000MS,
    FADE_2S,
    FADE_5S,
    FADE_10S,
    EFFECTS_COUNT  // optional, gives the number of effects
} effect_id_t;

#endif /* INC_EFFECTS_H_ */
