#include "can1.h"

namespace device{
namespace can{

CanOne::CanOne(const uint32_t can_id,device::can::Can::CanInitParams& can_init_params) : Can(can_id,can_init_params)
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