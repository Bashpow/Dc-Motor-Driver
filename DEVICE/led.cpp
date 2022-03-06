#include "led.h"
#include "stm32f0xx.h"

// PF0
// PF1

Led Led::_instance;

bool Led::LedInit(void) {
	if(led_init_state_ == false) {
		SetLed(LED_RED, false);
		SetLed(LED_GREEN, false);
		GPIO_InitTypeDef GPIO_InitStructure = {0};
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
		GPIO_Init(GPIOF, &GPIO_InitStructure);
		led_init_state_ = true;
	}
	return led_init_state_;
}

bool Led::GetLed(unsigned short led_x) const {
	return GPIO_ReadOutputDataBit(GPIOF, led_x);
}

void Led::SetLed(unsigned short led_x, bool state) const {
	if(state){
		GPIO_SetBits(GPIOF, led_x);
	} else {
		GPIO_ResetBits(GPIOF, led_x);
	}
}

void Led::ToggleLed(unsigned short led_x) const {
	GPIOF->ODR ^= led_x;
}
