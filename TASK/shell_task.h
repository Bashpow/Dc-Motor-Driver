#ifndef __SHELL_TASK_H_
#define __SHELL_TASK_H_

#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t* Get_Shell_Task_Handler(void);
void Create_Shell_Send_Task(void);

#endif //__SHELL_TASK_H_
