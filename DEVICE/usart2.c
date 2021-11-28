#include "usart2.h"

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
	while ((USART2->ISR & (uint32_t)0x80) != 0x80)
	{
	} //循环发送,直到发送完毕
	USART2->TDR = (uint8_t)ch;
	return ch;
}
#endif
static const uint16_t usart2_dma_rx_max_len = 14;
static char usart2_rx_buf[usart2_dma_rx_max_len];
static uint8_t usart2_rxd_length = 0;
static const uint16_t usart2_dma_send_max_len = 256;
static uint8_t usart2_dma_tx_buffer[usart2_dma_send_max_len];

void Usart2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	USART_InitTypeDef USART_InitStructure = {0};
	DMA_InitTypeDef DMA_InitStructure = {0};
	NVIC_InitTypeDef NVIC_InitStructure = {0};

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/**
	PA2   ------> USART2_TX
	PA3   ------> USART2_RX
	*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_Init(&NVIC_InitStructure);

	DMA_DeInit(DMA1_Channel5);
	DMA_DeInit(DMA1_Channel4);

	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART2->RDR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart2_rx_buf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = usart2_dma_rx_max_len;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;

	DMA_Init(DMA1_Channel5, &DMA_InitStructure);

	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->TDR);	///< 外设地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart2_dma_tx_buffer;	///< 内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;						///< dma传输方向单向
	DMA_InitStructure.DMA_BufferSize = usart2_dma_send_max_len;				///< 设置DMA在传输时缓冲区的长度
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		///< 设置DMA的外设递增模式，一个外设
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					///< 设置DMA的内存递增模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; ///< 外设数据字长
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;		///< 内存数据字长
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;							///< 设置DMA的传输模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;						///< 设置DMA的优先级别
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;							///< 设置DMA的2个memory中的变量互相访问

	DMA_Init(DMA1_Channel4, &DMA_InitStructure);

	///< DMA1_Channel4 USART2 Tx
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);

	///< 使能 DMA1_Channel5 USART2 Rx
	DMA_Cmd(DMA1_Channel5, ENABLE);

	///< 串口2空闲中断（一帧数据接收完毕）
	USART_ITConfig(USART2, USART_IT_IDLE, DISABLE);
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
	USART_Cmd(USART2, ENABLE);
}
/*

*/
void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		//关闭DMA
		USART_ClearITPendingBit(USART2, USART_IT_IDLE);
		DMA_Cmd(DMA1_Channel5, DISABLE);

		(void)USART2->ISR;
		(void)USART2->RDR;

		//计算接收到的数据长度
		usart2_rxd_length = usart2_dma_rx_max_len - DMA_GetCurrDataCounter(DMA1_Channel5);

		//重设传输长度
		DMA_SetCurrDataCounter(DMA1_Channel5, usart2_dma_rx_max_len);

		//开启DMA
		DMA_Cmd(DMA1_Channel5, ENABLE);
	}
}

void DMA1_Channel4_5_IRQHandler(void)
{
	if (DMA_GetFlagStatus(DMA1_FLAG_TC4) == SET)
	{
		DMA_Cmd(DMA1_Channel4, DISABLE);								///< 关闭DMA通道
		DMA_SetCurrDataCounter(DMA1_Channel4, usart2_dma_send_max_len); ///< 重设数据长度
		DMA_ClearFlag(DMA1_FLAG_TC4);									///< 清中断标志，否则会一直中断
	}
}

void Usart2_Dma_Send(uint32_t data_address, uint16_t size)
{
	DMA1_Channel4->CMAR = data_address;

	DMA_SetCurrDataCounter(DMA1_Channel4, size);
	USART_ClearFlag(USART2, USART_FLAG_TC);
	DMA_ClearFlag(DMA1_FLAG_TC4);
	DMA_Cmd(DMA1_Channel4, ENABLE);
}

uint8_t *Get_Usart2_Rxd_Data_Len(void)
{
	return &usart2_rxd_length;
}

char *Get_Usart2_Rxd_Data_Buffer(void)
{
	return usart2_rx_buf;
}

int ___printf(const char *format, ...)
{
	uint32_t len;
	va_list args;
	va_start(args, format);
	len = vsnprintf((char *)usart2_dma_tx_buffer, sizeof(usart2_dma_tx_buffer), (const char *)format, args);
	va_end(args);
	Usart2_Dma_Send((uint32_t)usart2_dma_tx_buffer, len);
	return -1;
}
