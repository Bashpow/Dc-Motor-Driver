#ifndef __USART2_H
#define __USART2_H

#include "stm32f10x.h"
#include "stdio.h"

#define USART2_SHELL_DMA_RX_MAX_SIZE ( 64u )

void Usart2_Init(uint32_t bound);
char* Get_Usart2_Shell_Rx_Buffer(void);
uint16_t Get_Usart2_Shell_Rx_Length(void);
void Usart2_Shell_Start_Dma_Receive(void);
void Usart2_DMA_Reset(void);

#endif
