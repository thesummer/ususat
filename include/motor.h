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
#include "Beagle_GPIO.h"

typedef void (cPWM::*SetDutyCyle)(unsigned int);

namespace USU
{

class Motor
{
public:
    Motor(Beagle_GPIO& beagleGpio, Beagle_GPIO::Pins clockwise,  Beagle_GPIO::Pins counterClockwise,
          cPWM& pwm, SetDutyCyle dutyCycle);
    void setSpeed(int speed);
    int getSpeed() const { return mSpeed; }

private:
    Beagle_GPIO& mBeagleGpio;
    Beagle_GPIO::Pins mClockwise;
    Beagle_GPIO::Pins mCounterClockwise;

    cPWM &mPwm;
    SetDutyCyle mSetDutyCycle;
    int mSpeed;
};

}

#endif // MOTOR_H
