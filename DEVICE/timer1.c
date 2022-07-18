#include "timer1.h"


/**
 * @brief 直流有刷电机PWM输出的初始化
 * @note 对应引脚：PA8 TIM1_CH1，PA9 TIM1_CH2，PA7 TIM1_CH1N，PB0 TIM1_CH2N
 * @param Period 对应输出的定时器的重装载计数器。输出PWM频率= (64M/TIM_Period) Mhz
 */
void Dc_Brush_Motor_Pwm_Init(uint16_t Period)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	TIM_DeInit(TIM1);
	TIM_InternalClockConfig(TIM1);
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9| GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA ,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB ,&GPIO_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_Period = Period;//根据需求设置即可
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;//根据需求配置
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//PWM1模式
	TIM_OCInitStructure.TIM_Pulse = 0;//占空比设置 根据需求设置即可
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//互补输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//输出极性
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//互补输出极性
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;//死区后输出状态
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;//死区后互补输出状态
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
	TIM_OC2Init(TIM1,&TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
	
	//死区和刹车功能配置，使用了高级定时器
	TIM_BDTRStructInit(&TIM_BDTRInitStructure);
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;//运行模式下输出选择
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;//空闲模式下输出选择
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;//锁定设置
	TIM_BDTRInitStructure.TIM_DeadTime = 16;//死区时间设置
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;//刹车功能使能
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;//刹车输入极性
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//自动输出使能
	TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE); //******输出使能必须要有

	TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);	// W上管
	TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Disable); // W下管
	TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);	// W上管
	TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Disable); // W下管

	TIM_Cmd(TIM1,ENABLE);
}

#include "usart2.h"

/**
 * @brief 设置电机...
 * 
 * @param power 
 * @return int8_t 
 */
int8_t Set_Motor_Power(int32_t power)
{
	if ((power > TIM1_MAX_ARR_VALUE) ||
		(power < (-TIM1_MAX_ARR_VALUE)))
	{
		return -1;
	}

	if(power == 0)
	{
		TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);//W上管
   		TIM_CCxNCmd(TIM1,TIM_Channel_1, TIM_CCxN_Disable);//W下管
		TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);//W上管
    	TIM_CCxNCmd(TIM1,TIM_Channel_2, TIM_CCxN_Disable);//W下管

		TIM1->CCR2 = 0;
		TIM1->CCR2 = 0;
	}
	else
	{
		if(power > 0)
		{
			TIM1->CCR1 = power;
			TIM1->CCR2 = 0;
		}
		else
		{
			TIM1->CCR1 = 0;
			TIM1->CCR2 = (-power);
		}

		TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);//W上管
		TIM_CCxNCmd(TIM1,TIM_Channel_1, TIM_CCxN_Enable);//W下管
		TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);//W上管
		TIM_CCxNCmd(TIM1,TIM_Channel_2, TIM_CCxN_Enable);//W下管
	}

	return 1;
}
