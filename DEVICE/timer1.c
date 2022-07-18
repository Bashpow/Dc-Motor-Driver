#include "timer1.h"


/**
 * @brief ֱ����ˢ���PWM����ĳ�ʼ��
 * @note ��Ӧ���ţ�PA8 TIM1_CH1��PA9 TIM1_CH2��PA7 TIM1_CH1N��PB0 TIM1_CH2N
 * @param Period ��Ӧ����Ķ�ʱ������װ�ؼ����������PWMƵ��= (64M/TIM_Period) Mhz
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA ,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB ,&GPIO_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_Period = Period;//�����������ü���
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;//������������
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//PWM1ģʽ
	TIM_OCInitStructure.TIM_Pulse = 0;//ռ�ձ����� �����������ü���
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//���ʹ��
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//�������ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//�������
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//�����������
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;//���������״̬
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;//�����󻥲����״̬
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
	TIM_OC2Init(TIM1,&TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
	
	//������ɲ���������ã�ʹ���˸߼���ʱ��
	TIM_BDTRStructInit(&TIM_BDTRInitStructure);
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;//����ģʽ�����ѡ��
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;//����ģʽ�����ѡ��
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;//��������
	TIM_BDTRInitStructure.TIM_DeadTime = 16;//����ʱ������
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;//ɲ������ʹ��
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;//ɲ�����뼫��
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//�Զ����ʹ��
	TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE); //******���ʹ�ܱ���Ҫ��

	TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);	// W�Ϲ�
	TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Disable); // W�¹�
	TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);	// W�Ϲ�
	TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Disable); // W�¹�

	TIM_Cmd(TIM1,ENABLE);
}

#include "usart2.h"

/**
 * @brief ���õ��...
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
		TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);//W�Ϲ�
   		TIM_CCxNCmd(TIM1,TIM_Channel_1, TIM_CCxN_Disable);//W�¹�
		TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);//W�Ϲ�
    	TIM_CCxNCmd(TIM1,TIM_Channel_2, TIM_CCxN_Disable);//W�¹�

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

		TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);//W�Ϲ�
		TIM_CCxNCmd(TIM1,TIM_Channel_1, TIM_CCxN_Enable);//W�¹�
		TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);//W�Ϲ�
		TIM_CCxNCmd(TIM1,TIM_Channel_2, TIM_CCxN_Enable);//W�¹�
	}

	return 1;
}
