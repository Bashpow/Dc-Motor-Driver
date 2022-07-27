#ifndef __CANDUMP_TASK_H_
#define __CANDUMP_TASK_H_

#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t* Get_Candump_Task_Handler(void);
void Create_Candump_Task(void);

#endif //__CANDUMP_TASK_H_
