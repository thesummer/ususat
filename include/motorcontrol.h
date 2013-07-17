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

#include "cPWM.h"
#include "Beagle_GPIO.h"
#include "motor.h"
#include "max127.h"
#include "messages.h"

namespace USU
{

/*!
 \brief Represents the class for motor control

 It initializes the interface to the 4 motors. It receives the last system state estimate from the Kalman
 filter, calculates the appropiate control response and sets the speed (duty cycle)
 of the motors.

 TODO: Get the desired state from ground station to calculate the control response.

*/
class MotorControl
{
public:


    /*!
     \brief Constructor of the class

     Initializes the underlying the GPIO-class,
     the PWMs the 4 Motors and the ADC.

     \param priority  priority of the periodic pthread
    */
    MotorControl(const char* i2cdevice="/dev/i2c-3");

    virtual ~MotorControl();

    /*!
     \brief Calculate the control responste from the current state estimate

     \param state the current state estimate from the Kalman filter
   */
    void calculateControlResponse(Quaternion state);

    /*!
     \brief For testing: sets the speed of a motor

     \param motor which motor [0..3]
     \param dutyCycle  which speed [-100..100]
    */
    void setMotor(int motor, int dutyCycle);


    /*!
     \brief For testing: returns the Analog measurements of a motor

     \param motor which motor [0..3]
     \param aOut1   reference to a variable to store the first analog measurement
     \param aOut2   reference to a variable to store the second analog measurement
    */
    void getAnalog(int motor, float &aOut1, float &aOut2);

    void getAnalogs(float * aOut1, float* aOut2);

    void getDutyCycles(int* dc);

private:
    cPWM mPwm1; /*!< First PWM module (has 2 channels) */
    cPWM mPwm2; /*!< Second PwM module (has 2 channels) */

    Beagle_GPIO mBeagleGpio; /*!< Representation of the BeagleBoard Gpios (access via mmap) */
    Motor *mMotor[4]; /*!< Array of the 4 motors */

    Max127 mAnalog; /*!< Representation of the ADC for the motor feedback */

    MotorControl(const MotorControl& thread); /*!< Copy constructor made inaccessible by declaring it private */

    MotorControl& operator=(const MotorControl& rhs); /*!< Assignment constructor made inaccessible by declaring it private */
};

}

#endif // MOTORCONTROL_H
