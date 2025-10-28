/*
 * Patterns.h
 *
 *  Created on: Oct 22, 2025
 *      Author: TIM
 */

#ifndef INC_PATTERNS_H_
#define INC_PATTERNS_H_

#include "cmd.h"

extern void sendMessage(cmd_t message);
extern uint8_t rand255(void);

void setColour(uint8_t RED, uint8_t GREEN, uint8_t BLUE);
void sweep(uint8_t RED, uint8_t GREEN, uint8_t BLUE);
void long_colour_sweep(void);
void vertical_lines_alternate(void);
void vertical_lines_fade_through(void);
void vertical_lines_fade_through_short(void);
void horizontal_lines_alternate(void);
void horizontal_lines_fade_through(void);
void horizontal_lines_fade_through_short(void);
void fluke_snake(void);
void snake(uint8_t RED, uint8_t GREEN, uint8_t BLUE);
void rain(uint8_t RED, uint8_t GREEN, uint8_t BLUE, int cycles, int speedMS);
void horizontal_zappies(uint8_t RED, uint8_t GREEN, uint8_t BLUE, int cycles, int speedMS);
void spiral_fill_in(void);
void diagonal_sweep(void);

#endif /* INC_PATTERNS_H_ */
