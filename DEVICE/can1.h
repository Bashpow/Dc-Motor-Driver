#ifndef DEVICE_CAN_ONE_H_
#define DEVICE_CAN_ONE_H_

#include "can.h"
#include "usart2.h"
#include "motor.h"

extern "C"
{
#include "stm32f0xx.h"
}

namespace device{
namespace can{

class CanOne :public CanInterface{

public:

    CanOne();
    void CanInit() override;
    void CanSendFourMessage(uint32_t id, int16_t data1, int16_t data2, int16_t data3, int16_t data4) ;
    
    static void CanOneFifo0RxDataItCallBack();
    static CanOne& GetCanOneSigleInstance(){
        static CanOne can_one_instance_;
        return can_one_instance_;
    }
private:
    CanOne(const CanOne&);
    ~CanOne() override {}
	CanOne& operator=(const CanOne&);
};

// 数据类型 类名：：静态成员变量名 = 初值；

} // namespace can
} // namespace device

#endif // DEVICE_CAN_ONE_H_