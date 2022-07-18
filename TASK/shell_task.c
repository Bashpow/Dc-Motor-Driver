#include "shell_task.h"
#include "usart2.h"

#define SHELL_TASK_PRIO (5)                             //任务优先级
#define SHELL_TASK_STK_SIZE (200)                       //任务堆栈大小
static StackType_t ShellTaskStack[SHELL_TASK_STK_SIZE]; //任务堆栈
static StaticTask_t ShellTaskTCB;                       //任务控制块
static TaskHandle_t ShellTask_Handler;                  //任务句柄
static void Shell_Task_Entry(void *pvParameters);

inline TaskHandle_t *Get_Shell_Task_Handler(void)
{
    return &ShellTask_Handler;
}

inline void Create_Shell_Send_Task(void)
{
    ShellTask_Handler = xTaskCreateStatic((TaskFunction_t)Shell_Task_Entry,
                                          (const char *)"shell",
                                          (uint32_t)SHELL_TASK_STK_SIZE,
                                          (void *)NULL,
                                          (UBaseType_t)SHELL_TASK_PRIO,
                                          (StackType_t *)ShellTaskStack,
                                          (StaticTask_t *)&ShellTaskTCB);
}

static void Shell_Task_Entry(void *pvParameters)
{
    for (;;)
    {
        printf("shell\r\n");
        vTaskDelay(2000);
    }
}
