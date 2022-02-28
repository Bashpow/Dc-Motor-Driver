#ifndef __USART2_H_
#define __USART2_H_

#include "stm32f0xx.h"
#include "stdio.h"
#include "stdarg.h"

void Usart2_Init(void);
uint8_t *Get_Usart2_Rxd_Data_Len(void);
char *Get_Usart2_Rxd_Data_Buffer(void);
void Usart2_Dma_Send(uint32_t data_address, uint16_t size);
int ___printf(const char *format, ...);

#endif // __USART2_H_
