#ifndef DEVICE_CAN_H_
#define DEVICE_CAN_H_
#include "stm32f0xx.h"

namespace device{
namespace can{

class CanInterface
{
public:
    CanInterface(){}
    virtual void CanInit() = 0;
    virtual ~CanInterface(){}
};

} // namespace can
} // namespace device

#endif // DEVICE_CAN_H_