#ifndef __TIMER1_H_
#define __TIMER1_H_

#include "stm32f10x.h"

// 由PWM频率计算定时器重装载寄存器值
#define TIM1_CALC_TIM_PERIOD(Pwm_Frequency) ( (uint16_t)(64000000 / Pwm_Frequency) )

// 定时器1设置的最大值
#define TIM1_MAX_ARR_VALUE ( (int32_t)6400 )

void Dc_Brush_Motor_Pwm_Init(uint16_t Period);
int8_t Set_Motor_Power(int32_t power);

#endif //__TIMER1_H_
