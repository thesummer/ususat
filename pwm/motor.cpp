/**
 * @file motor.cpp
 *
 * Class to represent a motor
 *
 * @author Jan Sommer
 *  Created on: Apr 22, 2013
 *
 */

#include "motor.h"
using namespace USU;

Motor::Motor(Beagle_GPIO &beagleGpio, Beagle_GPIO::GPIO_Pins clockwise, Beagle_GPIO::GPIO_Pins counterClockwise, SetDutyCyle dutyCycle)
    :mBeagleGpio(beagleGpio), mClockwise(clockwise),
     mCounterClockwise(counterClockwise), mSetDutyCycle(dutyCycle), mSpeed(0)
{
    mBeagleGpio.configurePin(mClockwise, Beagle_GPIO::kOUTPUT);
    mBeagleGpio.enablePinInterrupts( mClockwise, false );
    mBeagleGpio.writePin(mClockwise, 0);

    mBeagleGpio.configurePin(mCounterClockwise, Beagle_GPIO::kOUTPUT);
    mBeagleGpio.enablePinInterrupts( mCounterClockwise, false );
    mBeagleGpio.writePin(mCounterClockwise, 0);

    setSpeed(mSpeed);
}

void Motor::setSpeed(int speed)
{
    if (mSpeed > 0)
    {
        mBeagleGpio.writePin(mClockwise, 1);
        mBeagleGpio.writePin(mCounterClockwise, 0);
        mSetDutyCycle(speed);
        mSpeed = speed;
    }
    else if (dutyCycle < 0)
    {
        mBeagleGpio.writePin(mClockwise, 0);
        mBeagleGpio.writePin(mCounterClockwise, 1);
        mSetDutyCycle(-speed);
        mSpeed = speed;
    }
    else
    {
        mBeagleGpio.writePin(mClockwise, 0);
        mBeagleGpio.writePin(mCounterClockwise, 0);
        mSetDutyCycle(speed);
        mSpeed = 9;
    }
}