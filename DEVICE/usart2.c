#include "usart2.h"

// ͨ���ض���printf����
#if 1
#if !defined(__MICROLIB)
__asm(".global __use_no_semihosting\n\t");
void _sys_exit(int x) //����ʹ�ð�����ģʽ
{
	x = x;
}
//__use_no_semihosting was requested, but _ttywrch was
void _ttywrch(int ch)
{
	ch = ch;
}
FILE __stdout;
#endif
#if defined(__GNUC__) && !defined(__clang__)
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
	/* ʵ�ִ��ڷ���һ���ֽ����ݵĺ��� */
	USART2->TDR = ch;
	return ch;
}
#endif

static const uint16_t usart2_dma_rx_max_len = 14;
static char usart2_rx_buf[14];
static uint8_t usart2_rxd_length = 0;
static const uint16_t usart2_dma_send_max_len = 256;
static uint8_t usart2_dma_tx_buffer[256];

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

	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->TDR);	///< �����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart2_dma_tx_buffer;	///< �ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;						///< dma���䷽����
	DMA_InitStructure.DMA_BufferSize = usart2_dma_send_max_len;				///< ����DMA�ڴ���ʱ�������ĳ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		///< ����DMA���������ģʽ��һ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					///< ����DMA���ڴ����ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; ///< ���������ֳ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;		///< �ڴ������ֳ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;							///< ����DMA�Ĵ���ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;						///< ����DMA�����ȼ���
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;							///< ����DMA��2��memory�еı����������

	DMA_Init(DMA1_Channel4, &DMA_InitStructure);

	///< DMA1_Channel4 USART2 Tx
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);

	///< ʹ�� DMA1_Channel5 USART2 Rx
	DMA_Cmd(DMA1_Channel5, ENABLE);

	///< ����2�����жϣ�һ֡���ݽ�����ϣ�
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
		//�ر�DMA
		USART_ClearITPendingBit(USART2, USART_IT_IDLE);
		DMA_Cmd(DMA1_Channel5, DISABLE);

		(void)USART2->ISR;
		(void)USART2->RDR;

		//������յ������ݳ���
		usart2_rxd_length = usart2_dma_rx_max_len - DMA_GetCurrDataCounter(DMA1_Channel5);

		//���贫�䳤��
		DMA_SetCurrDataCounter(DMA1_Channel5, usart2_dma_rx_max_len);

		//����DMA
		DMA_Cmd(DMA1_Channel5, ENABLE);
	}
}

void DMA1_Channel4_5_IRQHandler(void)
{
	if (DMA_GetFlagStatus(DMA1_FLAG_TC4) == SET)
	{
		DMA_Cmd(DMA1_Channel4, DISABLE);								///< �ر�DMAͨ��
		DMA_SetCurrDataCounter(DMA1_Channel4, usart2_dma_send_max_len); ///< �������ݳ���
		DMA_ClearFlag(DMA1_FLAG_TC4);									///< ���жϱ�־�������һֱ�ж�
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
