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


MotorControl::MotorControl(KalmanFilter &kalmanfilter, int priority, unsigned int period_us)
    :PeriodicRtThread(priority, period_us), mPwm1(0), mPwm2(1), mKalmanFilter(kalmanfilter)
{
    // Initizalize the four motors
    ///TODO: use meaningful Pin numbers (declare consts)
    mMotor[0] = new Motor(mBeagleGpio, Beagle_GPIO::P8_1,Beagle_GPIO::P8_1, mPwm1, &cPWM::DutyA_percent);
    mMotor[1] = new Motor(mBeagleGpio, Beagle_GPIO::P8_1,Beagle_GPIO::P8_1, mPwm1, &cPWM::DutyB_percent);
    mMotor[2] = new Motor(mBeagleGpio, Beagle_GPIO::P8_1,Beagle_GPIO::P8_1, mPwm2, &cPWM::DutyA_percent);
    mMotor[3] = new Motor(mBeagleGpio, Beagle_GPIO::P8_1,Beagle_GPIO::P8_1, mPwm2, &cPWM::DutyB_percent);
}

void MotorControl::run()
{
    mKeepRunning = true;

    while(mKeepRunning)
    {
        bool temp = mKalmanFilter.getState();

        /// TODO: Make some control magic

        mMotor[0]->setSpeed(20);
        /// [...]

        waitPeriod();
    }
}


