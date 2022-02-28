#include "stm32f0xx.h"
#include "usart2.h"

void delay()
{
	uint32_t ad = 0;
	for (int i = 0; i < 90000; i++)
	{
		ad++;
	}
}

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

	// printf("woshinidie123\r\n");
	USART_ClearITPendingBit(USART2, USART_IT_IDLE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	while (1)
	{
		// Usart2_Dma_Send((uint32_t)data, 6);
		___printf("hello\r\n");
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
	/* Configure Flash prefetch */
	FLASH->ACR |= FLASH_ACR_PRFTBE;

	/* Pins PA11/12 instead of pins PA9/10 */
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_PA11_PA12_RMP;

	FLASH_SetLatency(FLASH_Latency_1);
	while ((0x1UL << (0U)) != ((((FLASH_TypeDef *)((0x40000000UL + 0x00020000UL) + 0x00002000UL))->ACR) & ((0x1UL << (0U)))))
	{
	}
	RCC_HSI14Cmd(ENABLE);

	/* Wait till HSI14 is ready */
	while ((((((RCC_TypeDef *)((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR2) & ((0x1UL << (1U)))) == (0x1UL << (1U))) != 1)
	{
	}
	(((((RCC_TypeDef *)((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR2)) = ((((((((RCC_TypeDef *)((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR2))) & (~((0x1FUL << (3U))))) | (16 << (uint32_t)3U))));
	RCC_HSI48Cmd(ENABLE);

	/* Wait till HSI48 is ready */
	while ((((((RCC_TypeDef *)((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR2) & ((0x1UL << (17U)))) == (0x1UL << (17U))) != 1)
	{
	}
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLKConfig(RCC_HCLK_Div1);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI48);

	/* Wait till System clock is ready */
	while ((uint32_t)((((RCC_TypeDef *)((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR) & ((0x3UL << (2U)))) != (0x0000000CU))
	{
	}
	SystemCoreClock = 48000000;

	RCC_HSI14ADCRequestCmd(ENABLE);
}
