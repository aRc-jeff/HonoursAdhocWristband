/*
 * Effects.h
 *
 *  Created on: Oct 22, 2025
 *      Author: TIM
 */

#ifndef INC_EFFECTS_H_
#define INC_EFFECTS_H_

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
