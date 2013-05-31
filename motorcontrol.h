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

#include "pwm/cPWM.h"
#include "pwm/Beagle_GPIO.h"
#include "pwm/motor.h"
#include "pwm/max127.h"

namespace USU
{

/*!
 \brief Represents the Periodic task for motor control

 This class is derived from PeriodicRtThread. It initializes the interface to the
 4 motors. In a periodic loop it takes the last system state estimate from the Kalman
 filter, calculates the appropiate control response and sets the speed (duty cycle)
 of the motors.
*/
class MotorControl
{
public:


    /*!
     \brief Constructor of the class

     Initializes the underlying PeriodicRtThread, the GPIO-class,
     the PWMs and the 4 Motors.

     \param priority  priority of the periodic pthread
     \param period_us   period (in us) of the periodic pthread
     \param kalmanfilter reference to the KalmanFilter to get state estimates
    */
    MotorControl(const char* i2cdevice="/dev/i2c-3");

    virtual ~MotorControl();

    /*!
     \brief Calculate the control responste from the current state estimate

     TODO: use a proper variable for the state estimate

     \param state the current state estimate from the Kalman filter
   */
    void calculateControlResponse(bool state);

    void setMotor(int motor, int dutyCycle);

    void printAnalog(int motor, float &aOut1, float &aOut2);

private:
    cPWM mPwm1; /*!< First PWM module (has 2 channels) */
    cPWM mPwm2; /*!< Second PwM module (has 2 channels) */

    Beagle_GPIO mBeagleGpio; /*!< Representation of the BeagleBoard Gpios (access via mmap) */
    Motor *mMotor[4]; /*!< Array of the 4 motors */

    Max127 mAnalog;

    MotorControl(const MotorControl& thread); /*!< Copy constructor made inaccessible by declaring it private */

    MotorControl& operator=(const MotorControl& rhs); /*!< Assignment constructor made inaccessible by declaring it private */
};

}

#endif // MOTORCONTROL_H
