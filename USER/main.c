#include "stm32f10x.h"
#include "led.h"

int main(void)
{
	Led_Init();
	for(;;)
	{
		LED_ONBOARD_TOGGLE();
		for(int i=0; i<10000000; i++){}
	}
}
