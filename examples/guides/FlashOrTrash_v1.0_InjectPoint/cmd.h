/*
 * cmd.h
 *
 *  Created on: Oct 22, 2025
 *      Author: TIM
 */

 #ifndef INC_CMD_H_
 #define INC_CMD_H_
 
 #include "clk.h"
 #include "board.h"
 #include "periph_conf.h"

 #define UNIVERSAL_TTL 0

 extern uint16_t cmdIndex;

 typedef struct
 {
     uint16_t cmdIndex;
     uint8_t TTL;
     uint8_t CMD;
     uint8_t RED;
     uint8_t GREEN;
     uint8_t BLUE;
     uint8_t P1X;
     uint8_t P1Y;
     uint8_t P2X;
     uint8_t P2Y;
     uint8_t blank1;
     uint8_t blank2;
     uint8_t blank3;
     bool queueable;
     bool valid;
 } cmd_t; 

 cmd_t cmd_create(uint8_t CMD,
                 uint8_t RED, uint8_t GREEN, uint8_t BLUE,
                 uint8_t P1X, uint8_t P1Y, uint8_t P2X, uint8_t P2Y,
                 bool queueable);
 
 #endif /* INC_CMD_H_ */
