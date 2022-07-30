#include "candump_task.h"
#include "can.h"
#include "timer1.h"
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
    for (;;)
    {
        // 等待任务通知
        if( ulTaskNotifyTake(pdTRUE, 1000) )
        {
            int16_t motor_power_to_set = 0;

            // 电调id冲突
            if(Get_Can_Receive_Message()->StdId == LOCAL_CAN_ID)
            {
                continue;
            }

            else if(Get_Can_Receive_Message()->StdId == CTRL_CAN_ID)
            {
                *(uint8_t*)(&motor_power_to_set) = Get_Can_Receive_Message()->Data[CAN_MSG_VALID_DATA_POSITION+1];
                *((uint8_t*)(&motor_power_to_set)+1) = Get_Can_Receive_Message()->Data[CAN_MSG_VALID_DATA_POSITION];

                Set_Motor_Power(motor_power_to_set);
            }
            else
            {
                ;
            }

        }

        else
        {
            Set_Motor_Power(0);
        }
    }
}
