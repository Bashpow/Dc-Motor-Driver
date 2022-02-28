#ifndef __MODULE_CONFIG_H_
#define __MODULE_CONFIG_H_

#include "stm32f0xx.h"
#include "stm32flash.h"

// 配置的成员参数
// 参考自Qt库的Q_OBJECT
#define CONFIG_MEMBER        \
    uint16_t z_phase;        \
    uint16_t mode;           \
    uint16_t can_id;         \
    uint16_t data_frequency; \
    uint16_t angle_max;      \
    uint16_t filter;         \
    uint16_t pwm_mode;       \
    int16_t pwm_range;       \
    int16_t motor_max_speed; \
    float pid_kp;            \
    float pid_ki;            \
    float pid_kd;

class ModuleConfig {
public:

private:
    ModuleConfig() : config_init_state_(false) { }
    ~ModuleConfig(){};

private:
    bool config_init_state_;
    struct {
        CONFIG_MEMBER
    } config_;
};

#endif // __MODULE_CONFIG_H_
