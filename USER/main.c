#include "stm32f10x.h"
#include "led.h"
#include "usart2.h"

extern uint32_t SystemCoreClock;

int main(void)
{
	// SystemCoreClockUpdate();
	// SystemCoreClockUpdate();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	Led_Init();
	Usart2_Init(115200);
#define info_print printf
// 	static void Print_Info(void)
// {
	///< 打印炫酷的提示信息
	info_print(" ___     _         _   \r\n");
	info_print("| _ \\___| |__  ___| |_ \r\n");
	info_print("|   / _ \\ '_ \\/ _ \\  _|\r\n");
	info_print("|_|_\\___/_.__/\\___/\\__|\r\n");
	info_print("\r\n");
	info_print("  ___         _           _ \r\n");
	info_print(" / __|___ _ _| |_ _ _ ___| |\r\n");
	info_print("| (__/ _ \\ ' \\  _| '_/ _ \\ |\r\n");
	info_print(" \\___\\___/_||_\\__|_| \\___/_|\r\n");
	info_print("\r\n");
	info_print(" ___                   _ \r\n");
	info_print("| _ ) ___  __ _ _ _ __| |\r\n");
	info_print("| _ \\/ _ \\/ _` | '_/ _` |\r\n");
	info_print("|___/\\___/\\__,_|_| \\__,_|\r\n");
	info_print("\r\n");
	info_print("+-+-+-+ +-+-+-+-+-+-+-+-+-+-+-+-+-+\r\n");
	info_print("|M|C|U| |S|T|M|3|2|F|1|0|3|C|8|T|6|\r\n");
	info_print("+-+-+-+ +-+-+-+-+-+-+-+-+-+-+-+-+-+\r\n");
	info_print("Mainstream Performance line, Arm Cortex-M3 MCU with 64 Kbytes of Flash memory, 72 MHz CPU, motor control, USB and CAN\r\n");
	info_print("\r\n");
// }
	for(;;)
	{
		RCC_ClocksTypeDef     RCC_Clocks;
    	RCC_GetClocksFreq(&RCC_Clocks);
		LED_ONBOARD_TOGGLE();
		printf("sysclock %d %d\r\n", RCC_GetSYSCLKSource(), SystemCoreClock);
		printf("sysclock %d %d\r\n", RCC_Clocks.SYSCLK_Frequency, RCC_Clocks.HCLK_Frequency);
		for(int i=0; i<10000000; i++){}
	}
}

#include "FreeRTOS.h"

static StaticTask_t  TldeTaskTCP;
static StackType_t   IdleTaskkStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
																		StackType_t ** ppxIdleTaskStackBuffer, 
																		uint32_t *pulIdleTaskStackSize ) 
{
	*ppxIdleTaskTCBBuffer = &TldeTaskTCP;
	*ppxIdleTaskStackBuffer = IdleTaskkStack;
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}


