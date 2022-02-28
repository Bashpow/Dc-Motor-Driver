#ifndef __MODULE_CONFIG_H_
#define __MODULE_CONFIG_H_

#include "stm32f0xx.h"
#include "stm32flash.h"

// 配置对应的结构体
// 结构体大小需为2byte的倍数
struct ModuleConfigData{
	uint16_t z_phase;
	uint16_t mode;
	uint16_t can_id;
	uint16_t data_frequency;
	uint16_t angle_max;
	uint16_t filter;
	uint16_t pwm_mode;
	int16_t pwm_range;
	int16_t motor_max_speed;
	float pid_kp;
	float pid_ki;
	float pid_kd;
};

// 电调模式的配置
// 使用单例模式，通过 ModuleConfig::GetInstance() 获取
class ModuleConfig {
public:
	bool ModuleConfigInit();
	const struct ModuleConfigData& GetModeleConfig() const{
		return config_;
	}

public:
	static ModuleConfig& GetInstance(){
		return _instance;
	}

private:
	static ModuleConfig _instance;
	ModuleConfig() : config_init_state_(false) {}

private:
	bool config_init_state_;
	struct ModuleConfigData config_;

	// 配置信息的大小为 (flash_write_16bit_len*2) byte
	static size_t flash_write_16bit_len_;

	// 配置信息存放于flash的位置的首地址，一般为最后一个扇区基地址
	static size_t flash_free_page_base_addr_;
};

#endif // __MODULE_CONFIG_H_
