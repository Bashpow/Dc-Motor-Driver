#ifndef DEVICE_CAN_H_
#define DEVICE_CAN_H_
#include "stm32f0xx.h"

namespace device{
namespace can{

class CanInterface
{
public:
    
    struct CanToBeSendMessage
    {
    	uint32_t can_bus_dev_id_;
    	int16_t data1_;
    	int16_t data2_;
    	int16_t data3_;
    	int16_t data4_;
    };
    struct CanInitParams
    {
        GPIO_InitTypeDef GPIO_InitStructure_t;
        CAN_InitTypeDef CAN_InitStructure_t;
        CAN_FilterInitTypeDef CAN_FilterInitStructure_t;
        NVIC_InitTypeDef NVIC_InitStructure_t;
    };

    explicit CanInterface(const uint32_t can_id, CanInitParams& can_init_params) : can_id_(can_id),
                                                    can_init_params_(can_init_params) {}
    virtual ~CanInterface() {}
    virtual bool CanInit() = 0;
    virtual bool CanSendFourMessage() = 0;
    const uint32_t GetCanDeviceId() const { return can_id_; }
    void SetSendMessage(CanToBeSendMessage &can_send_msg) 
    {
        this->can_send_message.can_bus_dev_id_ = can_send_msg.can_bus_dev_id_;
        this->can_send_message.data1_ = can_send_msg.data1_;
        this->can_send_message.data2_ = can_send_msg.data2_;
        this->can_send_message.data3_ = can_send_msg.data3_;
        this->can_send_message.data4_ = can_send_msg.data4_;
    };

private:
    const uint32_t can_id_;
    const CanInitParams can_init_params_;
    CanToBeSendMessage can_send_message;
};

}
}




#endif // DEVICE_CAN_H_