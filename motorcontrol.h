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

/*!
 \brief Represents the Periodic task for motor control

 This class is derived from PeriodicRtThread. It initializes the interface to the
 4 motors. In a periodic loop it takes the last system state estimate from the Kalman
 filter, calculates the appropiate control response and sets the speed (duty cycle)
 of the motors.
*/
class MotorControl: public PeriodicRtThread
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
    MotorControl(int priority = 0 , unsigned int period_us = 1000000,
                 KalmanFilter& kalmanfilter);

    /*!
     \brief Signals the thread to stop
    */
    void stop() { mKeepRunning = false; }

    /*!
     \brief Thread routine

     - Gets the newest estimate from KalmanFilter
     - Calculate the control response
     - Set the motor speed of the 4 Motors
     - wait for the next timer event
     TODO: Its only an idea, no actual implementation yet.
    */
    virtual void run();

private:
    cPWM::cPWM mPwm1; /*!< First PWM module (has 2 channels) */
    cPWM::cPWM mPwm2; /*!< Second PwM module (has 2 channels) */

    Beagle_GPIO mBeagleGpio; /*!< Representation of the BeagleBoard Gpios (access via mmap) */
    Motor mMotor[4]; /*!< Array of the 4 motors */
    KalmanFilter& mKalmanFilter; /*!< Reference to the KalmanFilter */
    volatile bool mKeepRunning; /*!< Indicates if the Thread should keep running. volatile to prevent optimizing */

    MotorControl(const MotorControl& thread); /*!< Copy constructor made inaccessible by declaring it private */

    MotorControl& operator=(const MotorControl& rhs); /*!< Assignment constructor made inaccessible by declaring it private */
};

}

#endif // MOTORCONTROL_H
