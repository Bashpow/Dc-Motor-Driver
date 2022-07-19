#include "peripheral_init.h"
#include "led.h"
#include "usart2.h"
#include "timer1.h"

/**
 * @brief 外设初始化
 * 
 */
inline void Peripheral_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	Led_Init();
	Usart2_Init(115200);
    // Print_Logo();
	Dc_Brush_Motor_Pwm_Init( TIM1_CALC_TIM_PERIOD(10000) );
}

/**
 * @brief 串口打印LOGO
 * 
 */
inline void Print_Logo(void)
{
    ///< 打印炫酷的提示信息
	printf(" __  __       _             \r\n");
	printf("|  \\/  | ___ | |_ ___  _ __ \r\n");
	printf("| |\\/| |/ _ \\| __/ _ \\| '__|\r\n");
	printf("| |  | | (_) | || (_) | |   \r\n");
	printf("|_|  |_|\\___/ \\__\\___/|_|   \r\n");
	printf("\r\n");
	
	printf(" ____       _                \r\n");
	printf("|  _ \\ _ __(_)_   _____ _ __ \r\n");
	printf("| | | | '__| \\ \\ / / _ \\ '__|\r\n");
	printf("| |_| | |  | |\\ V /  __/ |   \r\n");
	printf("|____/|_|  |_| \\_/ \\___|_|   \r\n");
	printf("\r\n");
	
	printf("Copyright (C) 2022 JunGe Unlimited Company or its affiliates.  \r\nAll Rights Reserved.\r\n");
	printf("\r\n");

	// 输出mcu所有时钟状态
    // RCC_ClocksTypeDef RCC_Clocks;
    // RCC_GetClocksFreq(&RCC_Clocks);
    // printf("SYSCLK_Frequency: %d\r\n", RCC_Clocks.SYSCLK_Frequency);
    // printf("HCLK_Frequency: %d\r\n", RCC_Clocks.HCLK_Frequency);
    // printf("PCLK1_Frequency: %d\r\n", RCC_Clocks.PCLK1_Frequency);
    // printf("PCLK2_Frequency: %d\r\n", RCC_Clocks.PCLK2_Frequency);
    // printf("ADCCLK_Frequency: %d\r\n", RCC_Clocks.ADCCLK_Frequency);
    // printf("\r\n");
}

