#ifndef __MODULE_CONFIG_H_
#define __MODULE_CONFIG_H_

#include "stm32f0xx.h"
#include "stm32flash.h"

typedef struct
{
	uint16_t z_phase;

	uint16_t mode;

	uint16_t can_id;
	uint16_t data_frequency;
	uint16_t angle_max;
	uint16_t filter;

	uint16_t pwm_mode;
	int16_t pwm_range; ///< PWM¿ØÖÆ·¶Î§Îª 1500-pwm_range ~ 1500+pwm_range
	int16_t motor_max_speed;
	float pid_kp;
	float pid_ki;
	float pid_kd;

}Module_config_t;

void Module_Config_Init(void);
void Module_Config_Write_Into_Flash(void);
uint8_t Module_Config_Check(Module_config_t* check_config);
const Module_config_t* Get_Module_Config(void);
Module_config_t* Get_Module_Config_New(void);

#endif // __MODULE_CONFIG_H_
