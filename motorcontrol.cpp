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


MotorControl::MotorControl(int priority, unsigned int period_us, KalmanFilter &kalmanfilter)
    :PeriodicRtThread(priority, period_us), mPwm1(0), mPwm2(1), mKalmanFilter(kalmanfilter)
{
    mMotor[0] = new Motor(mBeagleGpio, 1,2, &mPwm1.DutyA_percent);
    mMotor[1] = new Motor(mBeagleGpio, 1,2, &mPwm1.DutyB_percent);
    mMotor[2] = new Motor(mBeagleGpio, 1,2, &mPwm2.DutyA_percent);
    mMotor[3] = new Motor(mBeagleGpio, 1,2, &mPwm2.DutyB_percent);
}

void MotorControl::run()
{
    mKeepRunning = true;
    while(mKeepRunning)
    {
        bool temp = mKalmanFilter.getState();

        /// TODO: Make some control magic

        mMotor[0].setSpeed(20);
        /// [...]

        waitPeriod();
    }
}


