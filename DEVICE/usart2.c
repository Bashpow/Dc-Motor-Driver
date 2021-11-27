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
	while((USART2->ISR & (uint32_t)0x80) != 0x80){}//循环发送,直到发送完毕
	USART2->TDR = (uint8_t) ch;      
	return ch;
}
#endif
static const uint16_t usart2_dma_rx_max_len = 14;
static char usart2_rx_buf[usart2_dma_rx_max_len];
static uint8_t usart2_rxd_length = 0;


void Usart2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	USART_InitTypeDef USART_InitStructure = {0};
	DMA_InitTypeDef   DMA_InitStructure = {0};
	NVIC_InitTypeDef NVIC_InitStructure = {0};

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/**USART2 GPIO Configuration
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
	
	//串口2空闲中断（一帧数据接收完毕）
	USART_ITConfig(USART2, USART_IT_IDLE, DISABLE);
	
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
	USART_Cmd(USART2, ENABLE);
	
	
	DMA_DeInit(DMA1_Channel5);

	//while (DMA_GetCmdStatus(DMA1_Channel5) != DISABLE);

	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->RDR); 
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

	DMA_Cmd(DMA1_Channel5,ENABLE);
}
/*

*/
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) 
	{		
		//关闭DMA
		USART_ClearITPendingBit(USART2, USART_IT_IDLE);
		DMA_Cmd(DMA1_Channel5, DISABLE);

		(void)USART2->ISR;
		(void)USART2->RDR;
		
		//计算接收到的数据长度
		usart2_rxd_length = usart2_dma_rx_max_len - DMA_GetCurrDataCounter(DMA1_Channel5);
		
		//printf("data: %s, len: %d \r\n",usart2_rx_buf,usart2_rxd_length);
		//重设传输长度
		DMA_SetCurrDataCounter(DMA1_Channel5, usart2_dma_rx_max_len);
		
		//开启DMA
		DMA_Cmd(DMA1_Channel5, ENABLE);
	}
}

uint8_t *Get_Usart2_Rxd_Data_Len(void)
{
	return &usart2_rxd_length;
}

char *Get_Usart2_Rxd_Data_Buffer(void)
{
	return usart2_rx_buf;
}

