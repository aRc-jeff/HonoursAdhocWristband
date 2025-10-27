/*
 * WS2812_SPI.h
 *
 *  Created on: Oct 22, 2025
 *      Author: TIM
 */

#ifndef INC_WS2812_SPI_H_
#define INC_WS2812_SPI_H_


void setLED (int led, int RED, int GREEN, int BLUE);
void WS2812_Send (void);
void setOneLED (int RED, int GREEN, int BLUE);

#define LED_TIME_GUARD 5

#endif /* INC_WS2812_SPI_H_ */
