#include "syncTools.h"

mutex_t lock;
cond_t condition;
bool newCMDFlag = false;
tsrb_t rxCMDs;
cmd_t rxCMDbuff[QUEUE_SIZE];

void init_thread_tools(void){
    tsrb_init(&rxCMDs, (uint8_t*)rxCMDbuff, sizeof(rxCMDbuff));
    mutex_init(&lock);
    cond_init(&condition);
}
