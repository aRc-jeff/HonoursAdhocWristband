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

 typedef struct
 {
     uint16_t cmdIndex;
     uint8_t TTL;
     uint8_t CMD;
     uint8_t RED;
     uint8_t GREEN;
     uint8_t BLUE;
     bool valid;
 } cmd_t; 
 
 #endif /* INC_CMD_H_ */
