/**
 * @file motorcontrol.cpp
 *
 * C++ class for the calculation of the control response.
 * Based on the PeriodicRtThread class.
 *
 * @author Jan Sommer
 *  Created on: Apr 22, 2013
 *
 */

#include "motorcontrol.h"
using namespace USU;


MotorControl::MotorControl()
    :PeriodicRtThread(priority, period_us), mPwm1(0), mPwm2(1), mKalmanFilter(kalmanfilter)
{
    // Initizalize the four motors
    mMotor[0] = new Motor(mBeagleGpio, Beagle_GPIO::P8_31,Beagle_GPIO::P8_29, mPwm1, &cPWM::DutyA_percent);
    mMotor[1] = new Motor(mBeagleGpio, Beagle_GPIO::P8_27,Beagle_GPIO::P8_25, mPwm1, &cPWM::DutyB_percent);
    mMotor[2] = new Motor(mBeagleGpio, Beagle_GPIO::P8_23,Beagle_GPIO::P8_21, mPwm2, &cPWM::DutyA_percent);
    mMotor[3] = new Motor(mBeagleGpio, Beagle_GPIO::P8_18,Beagle_GPIO::P8_17, mPwm2, &cPWM::DutyB_percent);
}

void MotorControl::calculateControlResponse(bool state)
{
    /// TODO: Make some control magic
    mMotor[0]->setSpeed(20);
    /// [...]

}
