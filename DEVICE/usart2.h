#ifndef __USART2_H_
#define __USART2_H_

#include "stm32f0xx.h"
#include "stdio.h"
#include "stm32f0xx_dma.h"

void Usart2_Init(void);
uint8_t *Get_Usart2_Rxd_Data_Len(void);
char *Get_Usart2_Rxd_Data_Buffer(void);

#endif // __USART2_H_
