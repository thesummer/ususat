/**
 * @file motorcontrol.h
 *
 * C++ class for the calculation of the control response.
 * Based on the PeriodicRtThread class.
 *
 * @author Jan Sommer
 *  Created on: Apr 22, 2013
 *
 */

#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include "threading/periodicrtthread.h"
#include "pwm/cPWM.h"
using namespace cPWM;
#include "pwm/Beagle_GPIO.h"

namespace USU
{

class MotorControl: public PeriodicRtThread
{
public:


    MotorControl();

private:
    struct Motor
    {

    };

    cPWM mPwm1;
    cPWM mPwm2;

    Motor mMotor1;
    Motor mMotor2;
    Motor mMotor3;
    Motor mMotor4;


    void setMotor(enum Motor motor, int speed);
};

}

#endif // MOTORCONTROL_H
