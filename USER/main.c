#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "peripheral_init.h"
#include "cansend_task.h"
#include "shell_task.h"

/**
 * @brief 程序入口
 * 
 * @return int 
 */
int main(void)
{
	// 外设初始化
	Peripheral_Init();

	// 创建任务
	Create_Can_Send_Task();
	Create_Shell_Send_Task();

	// 开启任务调度器
	vTaskStartScheduler();
	for(;;);
}

