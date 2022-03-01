#ifndef DEVICE_CAN_ONE_H_
#define DEVICE_CAN_ONE_H_
#include "can.h"

namespace device{
namespace can{

class CanOne : public CanInterface{

public:
    explicit CanOne(const uint32_t can_id,device::can::CanInterface::CanInitParams& can_init_params);
    bool CanInit() override;
    bool CanSendFourMessage() override;
private:


};

}
}




#endif // DEVICE_CAN_ONE_H_