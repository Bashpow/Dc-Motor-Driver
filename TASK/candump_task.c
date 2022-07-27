#include "candump_task.h"
#include "can.h"
#include "usart2.h"

#define CANDUMP_TASK_PRIO (1)                               //任务优先级
#define CANDUMP_TASK_STK_SIZE (300)                         //任务堆栈大小
static StackType_t CandumpTaskStack[CANDUMP_TASK_STK_SIZE]; //任务堆栈
static StaticTask_t CandumpTaskTCB;                         //任务控制块
static TaskHandle_t CandumpTask_Handler;                    //任务句柄
static void Candump_Task_Entry(void *pvParameters);

inline TaskHandle_t *Get_Candump_Task_Handler(void)
{
    return &CandumpTask_Handler;
}

inline void Create_Candump_Task(void)
{
    CandumpTask_Handler = xTaskCreateStatic((TaskFunction_t)Candump_Task_Entry,
                                          (const char *)"candump",
                                          (uint32_t)CANDUMP_TASK_STK_SIZE,
                                          (void *)NULL,
                                          (UBaseType_t)CANDUMP_TASK_PRIO,
                                          (StackType_t *)CandumpTaskStack,
                                          (StaticTask_t *)&CandumpTaskTCB);
}

static void Candump_Task_Entry(void *pvParameters)
{
    vTaskDelay(200);
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
    	// CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

    for (;;)
    {
        // 等待任务通知
        if( ulTaskNotifyTake(pdTRUE, 1000) )
        {
            printf("id %d \r\n", Get_Can_Receive_Message()->StdId);
            printf("rx data\r\n");
        }

        else
        {
            printf("can out of time\r\n");
        }
    }
}
