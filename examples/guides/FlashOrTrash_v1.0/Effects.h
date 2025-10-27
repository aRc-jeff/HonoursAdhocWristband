/*
 * Effects.h
 *
 *  Created on: Oct 22, 2025
 *      Author: TIM
 */

#ifndef INC_EFFECTS_H_
#define INC_EFFECTS_H_

#include "cmd.h"

extern int (*effectFuncs[])(cmd_t);
extern uint8_t RED_BASE;
extern uint8_t GREEN_BASE;
extern uint8_t BLUE_BASE;


#endif /* INC_EFFECTS_H_ */
