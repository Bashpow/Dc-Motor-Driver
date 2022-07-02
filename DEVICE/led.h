#ifndef __LED_H_
#define __LED_H_

#include "stm32f10x.h"

#define LED_ONBOARD_ON()      GPIOB->BSRR = GPIO_Pin_12

#define LED_ONBOARD_OFF()     GPIOB->BRR  = GPIO_Pin_12

#define LED_ONBOARD_TOGGLE()  GPIOB->ODR ^= GPIO_Pin_12

void Led_Init(void);

#endif //__LED_H_
