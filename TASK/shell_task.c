#include "shell_task.h"
#include "usart2.h"
#include "peripheral_init.h"
#include "shell.h"

#define SHELL_TASK_PRIO (5)                             //�������ȼ�
#define SHELL_TASK_STK_SIZE (300)                       //�����ջ��С
static StackType_t ShellTaskStack[SHELL_TASK_STK_SIZE]; //�����ջ
static StaticTask_t ShellTaskTCB;                       //������ƿ�
static TaskHandle_t ShellTask_Handler;                  //������
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

static void Shell_Str_Process(char* rx_buffer, uint32_t rx_length);

static void Shell_Task_Entry(void *pvParameters)
{
    Print_Logo();
    Shell_Init();
    for (;;)
    {
        // �������ڽ���
        Usart2_Shell_Start_Dma_Receive();

        // �ȴ�����֪ͨ
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // shell����
        Shell_Str_Process(Get_Usart2_Shell_Rx_Buffer(), Get_Usart2_Shell_Rx_Length());
        Shell_Command_Parse(Get_Usart2_Shell_Rx_Buffer());
    }
}

/**
 * @brief �ַ���Ԥ����
 * 
 * @param rx_buffer ���յ��ַ���
 * @param rx_length �ַ�������
 */
static void Shell_Str_Process(char* rx_buffer, uint32_t rx_length)
{
	if(rx_buffer[rx_length-2]=='\r' && rx_buffer[rx_length-1]=='\n')
	{
		rx_buffer[rx_length-2] = '\0';
	}
	else
	{
		rx_buffer[rx_length] = '\0';
	}
}
