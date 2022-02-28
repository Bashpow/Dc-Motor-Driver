#ifndef DEVICE_CAN_H_
#define DEVICE_CAN_H_
#include "stm32f0xx.h"



class Can
{
public:
    explicit Can(const uint32_t can_id) : can_id_(can_id) {}
    virtual ~Can() {}
    virtual bool CanInit() = 0;
    virtual bool CanSendFourMessage(int16_t data1,int16_t data2,int16_t data3,int16_t data4) = 0;
    const uint32_t GetCanDeviceId() const { return can_id_; }
private:
    const uint32_t can_id_;
};





#endif // DEVICE_CAN_H_