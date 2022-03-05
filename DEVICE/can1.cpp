#include "can1.h"

extern "C"{
#include "stm32f0xx_gpio.h"
}

namespace device{
namespace can{

CanOne::CanOne() 
{
	/* empty */
}

void CanOne::CanInit()
{
  	GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  	GPIO_InitStructure.GPIO_Pin = GPIO_PinSource11 | GPIO_PinSource12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		
	// Init GPIO PA12,PA11
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_4);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_4);
	
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = ENABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_1tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_1tq;
	CAN_InitStructure.CAN_Prescaler = 16;
	//CAN BaudRate 48/(1+1+1)/16=1Mbps
	CAN_Init(CAN, &CAN_InitStructure);
	
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	
	//Can filter init
	CAN_FilterInit(&CAN_FilterInitStructure);
	// Open can FIFO 0 message pending Interrupt
	CAN_ITConfig(CAN, CAN_IT_FMP0, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = CEC_CAN_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void CanOne::CanSendFourMessage(uint32_t id, int16_t data1, int16_t data2, int16_t data3, int16_t data4)
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

	CAN_Transmit(CAN, &TxMessage);
}

// 进入中断后会调用该函数
void CanOne::CanOneFifo0RxDataItCallBack()
{ 
	static CanRxMsg can_one_rx_message;
	if (CAN_GetITStatus(CAN, CAN_IT_FMP0) != 0)
	{
		CAN_ClearITPendingBit(CAN, CAN_IT_FMP0);
		CAN_Receive(CAN, CAN_FIFO0, &can_one_rx_message);
		userlib::motor::Motor::CalculateMotorData(
			userlib::motor::Motor::GetMotorSignelInstance(),
			&can_one_rx_message);
	}
} 

void CEC_CAN_IRQHandler()
{
	device::can::CanOne::CanOneFifo0RxDataItCallBack();
}

} // namespace can
} // namespace device