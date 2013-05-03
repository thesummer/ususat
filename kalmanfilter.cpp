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

#include<iostream>

#include "kalmanfilter.h"
#include "minimu/vector.h"
#include "3dm/gx3communicator.h"
#include "3dm/messages.h"

using namespace USU;

KalmanFilter::KalmanFilter(int priority, unsigned int period_us, char *i2cBus)
    :PeriodicRtThread(priority, period_us), mImu(i2cBus),
      mGX3(priority-1, "/dev/ttyO4"), mKeepRunning(false)
{
}

void KalmanFilter::run()
{

    vector acc, mag, gyro;
    mKeepRunning = true;
    mGX3.start();
    while(mKeepRunning)
    {
        acc  = mImu.readAcc();
        mag  = mImu.readMag();
        gyro = mImu.readGyro();

        ///TODO: Do some Kalman-Filtering magic here

        if(mGX3.isEmpty() == false)
        {
            AccAngMag test = mGX3.front();
            /// TODO: some more magic

            mGX3.pop();
        }
        std::cout << "," << acc[0] << "," << acc[1] << "," << acc[2]
                  << "," << mag[0] << "," << mag[1] << "," << mag[2]
                  << "," << gyro[0] << "," << gyro[1] << "," << gyro[2] << std::endl;



        // Alwasy use mutex, when changing state
        mStateLock.lock();
            mState = !mState;
        mStateLock.unlock();

        waitPeriod();
    }

    mGX3.stop();

}

bool KalmanFilter::getState()
{
    ScopedLock scLock(mStateLock);
    return mState;
}
