#ifndef __ENCODER_H_
#define __ENCODER_H_

#include "stm32f0xx.h"

#define MAIN_CIRCLE_CM 42.5f
#define MOTION_CIRCLE_CM 0.85f
#define TIM_4CNT_CM 0.05f
#define TIM_CNT_CM 0.0125f

void TIM2_Encoder_Init(void);

int16_t Get_Motor_Ncircle(void);

#endif // __ENCODER_H_
