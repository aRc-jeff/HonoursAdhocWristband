/*
 * syncTools.h
 *
 *  Created on: Oct 22, 2025
 *      Author: TIM
 */

 #ifndef INC_SYNCTOOLS_H_
 #define INC_SYNCTOOLS_H_
 
#include "clk.h"
#include "board.h"
#include "periph_conf.h"
#include "cond.h"
#include "mutex.h"
#include "tsrb.h"
#include "cmd.h"
#define QUEUE_SIZE 256 //must be a power of 2

extern mutex_t lock;
extern cond_t condition;
extern bool newCMDFlag;
extern tsrb_t rxCMDs;
extern cmd_t rxCMDbuff[QUEUE_SIZE];
void init_thread_tools(void);
 
 #endif /* INC_SYNCTOOLS_H_ */
