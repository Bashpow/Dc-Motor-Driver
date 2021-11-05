#include "encoder.h"

#include "usart2.h"

///< 角度值最大值，角度值范围为0-65535
#define ANGLE_VALUE_MAX 65535

///< 有无Z相线
#define Z_PHASE_LINE 0

///< 滤波等级
#define FILTER_LEVEL 6

int16_t motor_ncircle; //轮子转的整数圈数

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
	
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//使用编码器模式3，上升下降都计数
	TIM_ICStructInit(&TIM_ICInitStructure);//将结构体中的内容缺省输入
	TIM_ICInitStructure.TIM_ICFilter = FILTER_LEVEL;  //选择输入比较滤波器
	TIM_ICInit(TIM2, &TIM_ICInitStructure);//将TIM_ICInitStructure中的指定参数初始化TIM3
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority= 1 ;//优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清除TIM3的更新标志位
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//运行更新中断
	
	TIM2->CNT = 0;
	TIM_Cmd(TIM2, ENABLE);   //启动TIM4定时器
}

//编码器接口模式     优先级--2   1  1
void TIM2_IRQHandler(void)
{ 	
	if(TIM2->SR&0x0001)//溢出中断
	{
        if( ((TIM2->CR1) & ((0x1UL << (4U)))) == (0x1UL << (4U)) )
        {
            motor_ncircle --;
        }
        else
        {
            motor_ncircle ++;
        }
        
        TIM2->SR&=~(1<<0);//清除中断标志位
	}
	
}

int16_t Get_Motor_Ncircle(void)
{
	return motor_ncircle* 65535 + TIM2->CNT;
}
