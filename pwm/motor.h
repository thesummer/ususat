/**
 * @file motor.h
 *
 * Class to represent a motor
 *
 * @author Jan Sommer
 *  Created on: Apr 22, 2013
 *
 */

#ifndef MOTOR_H
#define MOTOR_H

#include "cPWM.h"
using namespace cPWM;
#include "Beagle_GPIO.h"

typedef void (cPWM::*SetDutyCyle)(unsigned int);

namespace USU
{

class Motor
{
public:
    Motor(Beagle_GPIO& beagleGpio, Beagle_GPIO::GPIO_Pins clockwise,  Beagle_GPIO::GPIO_Pins counterClockwise,
          SetDutyCyle dutyCycle);
    void setSpeed(int speed);
    int getSpeed() const { return mSpeed; }

private:
    Beagle_GPIO& mBeagleGpio;
    Beagle_GPIO::GPIO_Pins mClockwise;
    Beagle_GPIO::GPIO_Pins mCounterClockwise;
    bool mDutyCycleA;
    int mSpeed;
    SetDutyCyle mSetDutyCycle;
};

}

#endif // MOTOR_H
