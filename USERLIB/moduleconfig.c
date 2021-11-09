#include "moduleconfig.h"

#define FLASH_WRITE_16BIT_LEN (sizeof(Module_config_t)/sizeof(uint16_t))
#define FLASH_FREE_PAGE_BASE_ADDR 0x08007C00
static Module_config_t config;
static Module_config_t config_new;

void Module_Config_Init(void)
{
	Stm32flash_Read(FLASH_FREE_PAGE_BASE_ADDR, (uint16_t*)&config_new, FLASH_WRITE_16BIT_LEN);
	if(0 == Module_Config_Check(&config_new))
	{
		Module_Config_Write_Into_Flash();
	}
	Stm32flash_Read(FLASH_FREE_PAGE_BASE_ADDR, (uint16_t*)&config, FLASH_WRITE_16BIT_LEN);
}

void Module_Config_Write_Into_Flash(void)
{
	Module_Config_Check(&config_new);
	Stm32flash_Write(FLASH_FREE_PAGE_BASE_ADDR, (uint16_t*)&config_new, FLASH_WRITE_16BIT_LEN);
}

uint8_t Module_Config_Check(Module_config_t* check_config)
{
	uint8_t check_state = 1;
	if(check_config->z_phase!=0 && check_config->z_phase!=1)
	{
		check_config->z_phase=0;
		check_state = 0;
	}

	if(check_config->mode!=1 && check_config->mode!=2)
	{
		check_config->mode=1;
		check_state = 0;
	}

	if(check_config->can_id < 1 || check_config->can_id > 16)
	{
		check_config->can_id=1;
		check_state = 0;
	}

	if(check_config->data_frequency < 10 || check_config->data_frequency > 1000)
	{
		check_config->data_frequency=100;
		check_state = 0;
	}

	if(check_config->filter > 15)
	{
		check_config->filter=0;
		check_state = 0;
	}

	if(check_config->pwm_mode!=1 && check_config->pwm_mode!=2)
	{
		check_config->pwm_mode=1;
		check_state = 0;
	}

	if(check_config->pwm_range < 500 || check_config->pwm_range > 1000)
	{
		check_config->pwm_range=500;
		check_state = 0;
	}

	if(check_config->motor_max_speed <= 0)
	{
		check_config->motor_max_speed = 255;
		check_state = 0;
	}

	return check_state;
}

const Module_config_t* Get_Module_Config(void)
{
	return &config;
}

Module_config_t* Get_Module_Config_New(void)
{
	return &config_new;
}
