#include "peripheral_init.h"
#include "led.h"
#include "usart2.h"

static void Print_Logo(void);

/**
 * @brief 外设初始化
 * 
 */
inline void Peripheral_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	Led_Init();
	Usart2_Init(115200);
    Print_Logo();
}

/**
 * @brief 串口打印LOGO
 * 
 */
static inline void Print_Logo(void)
{
    ///< 打印炫酷的提示信息
	printf(" ___     _         _   \r\n");
	printf("| _ \\___| |__  ___| |_ \r\n");
	printf("|   / _ \\ '_ \\/ _ \\  _|\r\n");
	printf("|_|_\\___/_.__/\\___/\\__|\r\n");
	printf("\r\n");
	printf("  ___         _           _ \r\n");
	printf(" / __|___ _ _| |_ _ _ ___| |\r\n");
	printf("| (__/ _ \\ ' \\  _| '_/ _ \\ |\r\n");
	printf(" \\___\\___/_||_\\__|_| \\___/_|\r\n");
	printf("\r\n");
	printf(" ___                   _ \r\n");
	printf("| _ ) ___  __ _ _ _ __| |\r\n");
	printf("| _ \\/ _ \\/ _` | '_/ _` |\r\n");
	printf("|___/\\___/\\__,_|_| \\__,_|\r\n");
	printf("\r\n");
	printf("+-+-+-+ +-+-+-+-+-+-+-+-+-+-+-+-+-+\r\n");
	printf("|M|C|U| |S|T|M|3|2|F|1|0|3|C|8|T|6|\r\n");
	printf("+-+-+-+ +-+-+-+-+-+-+-+-+-+-+-+-+-+\r\n");
	printf("Mainstream Performance line, Arm Cortex-M3 MCU with 64 Kbytes of Flash memory, 72 MHz CPU, motor control, USB and CAN\r\n");
	printf("\r\n");

    RCC_ClocksTypeDef RCC_Clocks;
    RCC_GetClocksFreq(&RCC_Clocks);
    printf("SYSCLK_Frequency: %d\r\n", RCC_Clocks.SYSCLK_Frequency);
    printf("HCLK_Frequency: %d\r\n", RCC_Clocks.HCLK_Frequency);
    printf("PCLK1_Frequency: %d\r\n", RCC_Clocks.PCLK1_Frequency);
    printf("PCLK2_Frequency: %d\r\n", RCC_Clocks.PCLK2_Frequency);
    printf("ADCCLK_Frequency: %d\r\n", RCC_Clocks.ADCCLK_Frequency);
    printf("\r\n");
}

