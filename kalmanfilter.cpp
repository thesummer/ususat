/**
 * @file kalmanfilter.cpp
 *
 * C++ class for the sensor fusion and stated estimated.
 * Based on the PeriodicRtThread class.
 *
 * @author Jan Sommer
 *  Created on: Apr 20, 2013
 *
 */

#include "kalmanfilter.h"
#include "minimu/vector.h"
using namespace USU;

KalmanFilter::KalmanFilter(int priority, unsigned int period_us, char *i2cBus)
    :PeriodicRtThread(priority, period_us), mImu(i2cBus), mKeepRunning(false)
{
}

void KalmanFilter::run()
{

    vector acc, mag, gyro;
    mKeepRunning = true;

    while(mKeepRunning)
    {
        acc  = mImu.readAcc();
        mag  = mImu.readMag();
        gyro = mImu.readGyro();

        ///TODO: Do some Kalman-Filtering magic here

        // Alwasy use mutex, when changing state
        mStateLock.lock();
            mState = !mState;
        mStateLock.unlock();

        waitPeriod();
    }

}

bool KalmanFilter::getState() const
{
    ScopedLock scLock(mStateLock);
    return mState;
}
