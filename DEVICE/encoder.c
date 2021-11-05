#include "encoder.h"

#include "usart2.h"

///< �Ƕ�ֵ���ֵ���Ƕ�ֵ��ΧΪ0-65535
#define ANGLE_VALUE_MAX 65535

///< ����Z����
#define Z_PHASE_LINE 0

///< �˲��ȼ�
#define FILTER_LEVEL 6

int16_t motor_ncircle; //����ת������Ȧ��

void TIM2_Encoder_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    TIM_ICInitTypeDef TIM_ICInitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    /**TIM2 GPIO Configuration
    PA0   ------> TIM2_CH1
    PA1   ------> TIM2_CH2
    */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_2);
	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = ANGLE_VALUE_MAX;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//ʹ�ñ�����ģʽ3�������½�������
	TIM_ICStructInit(&TIM_ICInitStructure);//���ṹ���е�����ȱʡ����
	TIM_ICInitStructure.TIM_ICFilter = FILTER_LEVEL;  //ѡ������Ƚ��˲���
	TIM_ICInit(TIM2, &TIM_ICInitStructure);//��TIM_ICInitStructure�е�ָ��������ʼ��TIM3
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority= 1 ;//���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//���TIM3�ĸ��±�־λ
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//���и����ж�
	
	TIM2->CNT = 0;
	TIM_Cmd(TIM2, ENABLE);   //����TIM4��ʱ��
}

//�������ӿ�ģʽ     ���ȼ�--2   1  1
void TIM2_IRQHandler(void)
{ 	
	if(TIM2->SR&0x0001)//����ж�
	{
        if( ((TIM2->CR1) & ((0x1UL << (4U)))) == (0x1UL << (4U)) )
        {
            motor_ncircle --;
        }
        else
        {
            motor_ncircle ++;
        }
        
        TIM2->SR&=~(1<<0);//����жϱ�־λ
	}
	
}

int16_t Get_Motor_Ncircle(void)
{
	return motor_ncircle* 65535 + TIM2->CNT;
}
