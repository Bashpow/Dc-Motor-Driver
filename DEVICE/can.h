#ifndef __CAN_H_
#define __CAN_H_

#include "stm32f10x.h"

void Can1_Init(void);
void Can_Send_4Msg(uint32_t id, int16_t data1, int16_t data2, int16_t data3, int16_t data4);

#endif // __CAN_H_
