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

MotorControl::MotorControl(const char *i2cdevice)
    :mPwm1(1), mPwm2(2), mAnalog(i2cdevice)
{
    // Initizalize the four motors
    mMotor[0] = new Motor(mBeagleGpio, Beagle_GPIO::P8_31,Beagle_GPIO::P8_29, mPwm1, &cPWM::DutyA_percent);
    mMotor[1] = new Motor(mBeagleGpio, Beagle_GPIO::P8_27,Beagle_GPIO::P8_25, mPwm1, &cPWM::DutyB_percent);
    mMotor[2] = new Motor(mBeagleGpio, Beagle_GPIO::P8_23,Beagle_GPIO::P8_21, mPwm2, &cPWM::DutyA_percent);
    mMotor[3] = new Motor(mBeagleGpio, Beagle_GPIO::P8_18,Beagle_GPIO::P8_17, mPwm2, &cPWM::DutyB_percent);
    mPwm1.Period_freq(100);
    mPwm2.Period_freq(100);
    mPwm1.RunA();
    mPwm1.RunB();
    mPwm2.RunA();
    mPwm2.RunB();
}

MotorControl::~MotorControl()
{
    mPwm1.StopA();
    mPwm1.StopB();
    mPwm2.StopA();
    mPwm2.StopB();
}

void MotorControl::calculateControlResponse(Quaternion state)
{
    /// TODO: Make some control magic
    mMotor[0]->setSpeed(20);
    /// [...]

}

void MotorControl::setMotor(int motor, int dutyCycle)
{
    mMotor[motor]->setSpeed(dutyCycle);
}

void MotorControl::getAnalog(int motor, float& aOut1, float& aOut2)
{
    aOut1 =  mAnalog.readVoltage(motor*2);
    aOut2 =  mAnalog.readVoltage(motor*2 + 1);
}
