#ifndef _MOTOT_H_
#define _MOTOT_H_

extern "C"
{
#include "stm32f0xx.h"
}

namespace userlib{
namespace motor{

// TODO
class Motor; 
Motor&
  __doapl (Motor* ths, const Motor& r);
Motor& 
  __doami (Motor* ths, const Motor& r);
Motor&
  __doaml (Motor* ths, const Motor& r);


class Motor
{
private:
    Motor(const Motor&);
    Motor& operator=(const Motor&);
    ~Motor();
    
public:
    uint16_t mechanical_angle_;
    uint16_t speed_rpm_;
    uint16_t actual_torque_current_;
    uint16_t temperature_;
    Motor();
    Motor& operator +=(const Motor&);
    Motor& operator -=(const Motor&);
    Motor& operator *=(const Motor&);
    Motor& operator /=(const Motor&);

    static Motor& GetMotorSignelInstance(){
        static Motor motor_instance_;
        return motor_instance_; 
    }
    static void CalculateMotorData(Motor& motor, CanRxMsg* can_message)
    {
	    motor.mechanical_angle_ = (uint16_t)((can_message)->Data[0] << 8 | (can_message)->Data[1]);
	    motor.speed_rpm_ = (uint16_t)((can_message)->Data[2] << 8 | (can_message)->Data[3]);
	    motor.actual_torque_current_ = (uint16_t)((can_message)->Data[4] << 8 | (can_message)->Data[5]);
	    motor.temperature_ = (can_message)->Data[6];
    }
};

} // namespace motor
} // namespace userlib

#endif // _MOTOT_H_
