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
//using namespace cPWM;
#include "pwm/Beagle_GPIO.h"
#include "pwm/motor.h"

#include "kalmanfilter.h"

namespace USU
{

class MotorControl: public PeriodicRtThread
{
public:


    MotorControl(int priority = 0 , unsigned int period_us = 1000000,
                 KalmanFilter& kalmanfilter);

    void stop() { mKeepRunning = false; }
    virtual void run();

private:
    cPWM::cPWM mPwm1;
    cPWM::cPWM mPwm2;

    Beagle_GPIO mBeagleGpio;
    Motor mMotor[4];
    KalmanFilter& mKalmanFilter;
    volatile bool mKeepRunning;

    MotorControl(const MotorControl& thread); /*!< Copy constructor made inaccessible by declaring it private */
    MotorControl& operator=(const MotorControl& rhs); /*!< Assignment constructor made inaccessible by declaring it private */
};

}

#endif // MOTORCONTROL_H
