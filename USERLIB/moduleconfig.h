#ifndef __MODULE_CONFIG_H_
#define __MODULE_CONFIG_H_

#include "stm32f0xx.h"
#include "stm32flash.h"

// ���ö�Ӧ�Ľṹ��
// �ṹ���С��Ϊ2byte�ı���
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

// ���ģʽ������
// ʹ�õ���ģʽ��ͨ�� ModuleConfig::GetInstance() ��ȡ
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

	// ������Ϣ�Ĵ�СΪ (flash_write_16bit_len*2) byte
	static size_t flash_write_16bit_len_;

	// ������Ϣ�����flash��λ�õ��׵�ַ��һ��Ϊ���һ����������ַ
	static size_t flash_free_page_base_addr_;
};

#endif // __MODULE_CONFIG_H_
