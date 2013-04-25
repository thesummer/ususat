/**
 * @file kalmanfilter.h
 *
 * C++ class for the sensor fusion and stated estimated.
 * Based on the PeriodicRtThread class.
 *
 * @author Jan Sommer
 *  Created on: Apr 20, 2013
 *
 */



#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include "threading/periodicrtthread.h"
#include "minimu/minimu.h"
#include "threading/Lock.h"

namespace USU
{

/*!
 \brief Represents the Periodic class for state estimation

 This class is derived from PeriodicRtThread. It initializes the interface to the
 MinIMU9v2 and estimates the system state using Kalman filtering techniques. The state
 estimate can be accessed from other threads (protected by mutex).

 TODO:
    - Add interface to 3DM-GX3-25
    - Add interface to star camera
*/
class KalmanFilter : public PeriodicRtThread
{
public:
    /*!
     \brief Constructor of the class

     Initializes the interface to the MinIMU9 sensors

     \param priority    priority of the underlying periodic thread
     \param period_us   period (in us) of the underlying periodic thread
     \param i2cBus  name of the I2C-device (e.g. /dev/i2c-1)
    */
    KalmanFilter(int priority, unsigned int period_us, char* i2cBus);

    /*!
     \brief Thread routine

     - Gets sensor data from MinIMU9
     - Calculate state estimate
     - wait for next timer event

    TODO: Its only an idea no actual implementation yet.

    */
    virtual void run();

    /*!
     \brief Signals the thread to stop
    */
    void stop() { mKeepRunning = false; }

    /*!
     \brief Returns the current system state estimate

     Copies the current system state estimate. Acquires
     mutex before acessing the internal variable to avoid
     read/write-conflicts.

     \return bool Current system state
     TODO: Currently only dummy variable. Replace with actual state representation (quaternion?)
    */
    bool getState() const;

private:

    KalmanFilter(const KalmanFilter& thread); /*!< Copy constructor made inaccessible by declaring it private */
    KalmanFilter& operator=(const KalmanFilter& rhs); /*!< Assignment constructor made inaccessible by declaring it private */

    MinImu mImu; /*!< Class for accessing the MinIMU9v2*/
    ///TODO: semaphore for 3DM
    ///TODO: semaphore to ethernet
    bool mState; ///TODO: Actual representation of the state estimate /*!< TODO */
    Lock mStateLock; /*!< Mutex to access the State variable*/
    volatile bool mKeepRunning; /*!< Indicates if the Thread should keep running. volatile to prevent optimizing */
};

}

#endif // KALMANFILTER_H
