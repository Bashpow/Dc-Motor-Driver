#include "usart2.h"
#include "string.h"
#include "shell_task.h"

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{
	x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
	//循环发送,直到发送完毕
	while ((USART2->SR & 0X40) == 0)
	{
		vTaskDelay(1);
	}
	USART2->DR = (u8) ch;
	return ch;
}
#endif

static char usart2_shell_rx_buffer[USART2_SHELL_DMA_RX_MAX_SIZE] = {0};

/**
 * @brief 获取串口2的接受buffer
 * 
 * @return char* 接受到的字符数据
 */
inline char* Get_Usart2_Shell_Rx_Buffer(void)
{
	return usart2_shell_rx_buffer;
}

static uint16_t usart2_shell_rx_length = 0;

/**
 * @brief 获取当前shell接受到的数据长度
 * 
 * @return uint16_t 长度
 */
inline uint16_t Get_Usart2_Shell_Rx_Length(void)
{
	return usart2_shell_rx_length;
}

//Usart2    PA2--TX    PA3--RX
inline void Usart2_Init(uint32_t bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA，DMA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	//USART1_TX   GPIOA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9

	//USART2_RX	  GPIOA.3初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10

	// USART 初始化设置
	USART_DeInit(USART2);
	USART_InitStructure.USART_BaudRate = bound;										//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;					//收发模式
	USART_Init(USART2, &USART_InitStructure);										//初始化串口1
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
	USART_Cmd(USART2, ENABLE);														//使能串口1
	
	// USART2_RX DMA1_Channel6
	DMA_DeInit(DMA1_Channel6);
	DMA_StructInit(&DMA_InitStructure);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(USART2->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart2_shell_rx_buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 0;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel6, DISABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	(void)USART2->SR;
	(void)USART2->DR;
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
}

/**
 * @brief 开始使用DMA方式接收串口2
 * 
 */
inline void Usart2_Shell_Start_Dma_Receive(void)
{
	DMA_SetCurrDataCounter(DMA1_Channel6, USART2_SHELL_DMA_RX_MAX_SIZE); //重设传输长度
	DMA_ClearITPendingBit(DMA1_IT_TC6);
	DMA_Cmd(DMA1_Channel6, ENABLE); //开启DMA
}

/**
 * @brief 重置串口2DMA接收
 * 
 */
inline void Usart2_DMA_Reset(void)
{
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	USART_Cmd(USART2, DISABLE);
	DMA_Cmd(DMA1_Channel6, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel6, USART2_SHELL_DMA_RX_MAX_SIZE);
	memset(Get_Usart2_Shell_Rx_Buffer(), 0u, USART2_SHELL_DMA_RX_MAX_SIZE);
	USART_Cmd(USART2, ENABLE);
    DMA_ClearITPendingBit(DMA1_IT_TC6);
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) 
	{
		//关闭DMA
		DMA_Cmd(DMA1_Channel6, DISABLE);

		// 清除串口IDLE中断
		(void)USART2->SR;
		(void)USART2->DR;
		
		//计算接收到的数据长度
		usart2_shell_rx_length = USART2_SHELL_DMA_RX_MAX_SIZE - DMA_GetCurrDataCounter(DMA1_Channel6);

		//通知Shell系统任务解析
		// rt_sem_release(Get_Shell_Rx_Semaphore());
		vTaskNotifyGiveFromISR(*Get_Shell_Task_Handler(), NULL);
	}
}

