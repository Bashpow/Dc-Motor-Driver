#include "can.h"
#include "candump_task.h"

/*
 * PA11 --> CAN1_RX
 * PA12 --> CAN1_TX
*/

void Can1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = ENABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
	CAN_InitStructure.CAN_Prescaler = 4;
	CAN_Init(CAN1, &CAN_InitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void Can_Send_4Msg(uint32_t id, int16_t data1, int16_t data2, int16_t data3, int16_t data4)
{
	CanTxMsg TxMessage;
	TxMessage.StdId = id;
	TxMessage.IDE = CAN_Id_Standard;
	TxMessage.RTR = CAN_RTR_Data;
	TxMessage.DLC = 0x08;
	TxMessage.Data[0] = data1 >> 8;
	TxMessage.Data[1] = data1;
	TxMessage.Data[2] = data2 >> 8;
	TxMessage.Data[3] = data2;
	TxMessage.Data[4] = data3 >> 8;
	TxMessage.Data[5] = data3;
	TxMessage.Data[6] = data4 >> 8;
	TxMessage.Data[7] = data4;

	CAN_Transmit(CAN1, &TxMessage);
}

/**
 * @brief ͨ������canId��ȡ���Ƶ�����ĵ�id
 * 
 * @param local_can_id ����id
 * @return uint32_t ���Ʊ��ĵ�id
 */
uint32_t Calc_Can_Id_For_Control(uint32_t local_can_id)
{
	if(local_can_id>=0x221 && local_can_id<=0x224)
		return 0x220;
	
	else if(local_can_id>=0x225 && local_can_id<=0x228)
		return 0x21F;
	
	else if(local_can_id>=0x229 && local_can_id<=0x22C)
		return 0x21E;
	
	else if(local_can_id>=0x22D && local_can_id<=0x230)
		return 0x21D;
	
	return 0;
}

/**
 * @brief ��ȡ����id��Ӧ����Ч����λ��λ�ã�
 * 
 * @param local_can_id 
 * @return uint8_t 
 */
uint8_t Calc_Can_Msg_Valid_Data_Position(uint32_t local_can_id)
{
	if(local_can_id<0x221 || local_can_id>0x230)
		return 0u;
	return (((local_can_id - 0x221) % 4) * 2);
}

static CanRxMsg can_receive_message;

/**
 * @brief ��ȡ�յ���can����
 * 
 * @return const CanRxMsg* can����ָ��
 */
inline const CanRxMsg* Get_Can_Receive_Message(void)
{
	return &can_receive_message;
}

//Can1�����ж�
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		CAN_Receive(CAN1, CAN_FIFO0, &can_receive_message);
		
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(*Get_Candump_Task_Handler(), &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken ); //���Ϊ pdTRUE�������һ���������л�
	}
}
