#include "stm32f0xx.h"
#include "usart2.h"
#include "can1.h"
#include "rtthread.h"

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	Usart2_Init();
	
	device::can::CanOne &can_signle_instance = device::can::CanOne::GetCanOneSigleInstance();
	can_signle_instance.CanInit();
	// printf("woshinidie123\r\n");
	USART_ClearITPendingBit(USART2, USART_IT_IDLE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	while (1)
	{
		// Usart2_Dma_Send((uint32_t)data, 6);
		// ___printf("hello %d\r\n",SystemCoreClock);
		GPIO_SetBits(GPIOB, GPIO_Pin_1);
		rt_thread_delay(1000);
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);
		rt_thread_delay(1000);
	}
}
