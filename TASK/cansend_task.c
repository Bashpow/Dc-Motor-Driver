#include "cansend_task.h"
#include "can.h"
#include "led.h"
#include "usart2.h"

//任务优先级
#define TASK1_TASK_PRIO		2
//任务堆栈大小	
#define TASK1_STK_SIZE 		50
//任务堆栈
static StackType_t Task1TaskStack[TASK1_STK_SIZE];
static StaticTask_t Task1TaskTCB;
void Can_Send_Task(void *pvParameters);

TaskHandle_t* Get_Can_Send_Task_Handler(void)
{
    static TaskHandle_t Task1Task_Handler;
    return &Task1Task_Handler;
}

inline void Create_Can_Send_Task(void)
{
	*Get_Can_Send_Task_Handler()=xTaskCreateStatic((TaskFunction_t	)Can_Send_Task,		
										(const char* 	)"can_tx",
										(uint32_t 		)TASK1_STK_SIZE,	
										(void* 		  	)NULL,				
										(UBaseType_t 	)TASK1_TASK_PRIO, 	
										(StackType_t*   )Task1TaskStack,	
										(StaticTask_t*  )&Task1TaskTCB);
}




void Can_Send_Task(void *pvParameters)
{uint16_t test = 0;
	Can1_Init();
	for(;;)
	{
		Can_Send_4Msg(0x200, 0x1234, 0x5678, 0x1234, test++);
        LED_ONBOARD_TOGGLE();
		vTaskDelay(10); //100mhz
	}
}

