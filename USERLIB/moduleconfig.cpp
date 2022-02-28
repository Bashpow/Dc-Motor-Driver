#include "moduleconfig.h"

ModuleConfig ModuleConfig::_instance;
size_t ModuleConfig::flash_write_16bit_len_ = sizeof(struct ModuleConfigData)/sizeof(uint16_t);
size_t ModuleConfig::flash_free_page_base_addr_ = 0x08007C00;

bool ModuleConfig::ModuleConfigInit() {
    if(config_init_state_ == false){
        // 测试时定死配置信息
        this->config_.mode = 1;
        config_init_state_ = true;
    }
    return config_init_state_;
}

