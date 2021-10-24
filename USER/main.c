#include <stm32f0xx.h>

void delay(unsigned int time)
{
	while(time--);
	
	return;
}

int main()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	
	while(1)
	{
		delay(1000000);
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);
		delay(1000000);
		GPIO_SetBits(GPIOB, GPIO_Pin_1);
	}
	
	// return 0;
}
