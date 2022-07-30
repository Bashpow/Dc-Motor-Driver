#ifndef __CAN_H_
#define __CAN_H_

#include "stm32f10x.h"

// 0x221 - 0x230
#define LOCAL_CAN_ID ( (uint32_t) 0x222 )
#define CTRL_CAN_ID (Calc_Can_Id_For_Control(LOCAL_CAN_ID))
#define CAN_MSG_VALID_DATA_POSITION (Calc_Can_Msg_Valid_Data_Position(LOCAL_CAN_ID))

uint32_t Calc_Can_Id_For_Control(uint32_t local_can_id);
uint8_t Calc_Can_Msg_Valid_Data_Position(uint32_t local_can_id);

void Can1_Init(void);
void Can_Send_4Msg(uint32_t id, int16_t data1, int16_t data2, int16_t data3, int16_t data4);
const CanRxMsg* Get_Can_Receive_Message(void);

#endif // __CAN_H_
