#include "can1.h"

namespace device{
namespace can{

CanOne::CanOne(const uint32_t can_id,
    device::can::CanInterface::CanInitParams& can_init_params) 
    :CanInterface(can_id, can_init_params)
{

}

bool CanOne::CanInit()
{
    return false;
}

bool CanOne::CanSendFourMessage()
{
    return false;
}




} // namespace can
 
} // namespace device