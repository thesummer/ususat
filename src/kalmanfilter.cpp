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
using std::cout;
using std::endl;

#include <sys/time.h>
#include <unistd.h>


#include "kalmanfilter.h"
#include "vector.h"

using namespace USU;

int timeval_subtract (struct timeval * result, struct timeval * x, struct timeval * y)
{
    /* Perform the carry for the later subtraction by updating y. */
    if (x->tv_usec < y->tv_usec) {
        int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
        y->tv_usec -= 1000000 * nsec;
        y->tv_sec += nsec;
    }
    if (x->tv_usec - y->tv_usec > 1000000) {
        int nsec = (x->tv_usec - y->tv_usec) / 1000000;
        y->tv_usec += 1000000 * nsec;
        y->tv_sec -= nsec;
    }

    /* Compute the time remaining to wait.
          tv_usec is certainly positive. */
    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_usec = x->tv_usec - y->tv_usec;

    /* Return 1 if result is negative. */
    return x->tv_sec < y->tv_sec;
}

KalmanFilter::KalmanFilter(int priority, unsigned int period_us, const char *i2cImu, const char *i2cMotor)
    :PeriodicRtThread(priority, period_us), mImu(i2cImu),
      mGX3(priority, "/dev/ttyO4"), mMotors(i2cMotor), mKeepRunning(false)
{
}

void KalmanFilter::run()
{

    vector acc, mag, gyro;
    mKeepRunning = true;
    struct timeval start, now, elapsed;

//    mImu.enable();

    // Start the mGX3, put into continuous mode
    // then wait some time for the transmission to start
    mGX3.start();

    while(mGX3.isEmpty())
        usleep(1000);

    usleep(8000);


    gettimeofday(&start, NULL);
    while(mKeepRunning)
    {

//        acc  = mImu.readAcc();
//        mag  = mImu.readMag();
//        gyro = mImu.readGyro();

        gettimeofday(&now, NULL);
        timeval_subtract(&elapsed, &now, &start);
        unsigned long long timestamp = elapsed.tv_sec * 1000 + elapsed.tv_usec / 1000; // in ms since start
        Quaternion lastState;

        ///TODO: Do some Kalman-Filtering magic here

        if(mGX3.isEmpty() == false)
        {
            lastState = mGX3.front();
            mGX3.pop();

            /// TODO: some more magic
        }
        else
        {
            std::cout << "Missed measurement" << std::endl;
        }     
        std::cout << timestamp << "\t" << lastState.quat.w() << "\t" << lastState.quat.x() << "\t"
                                       << lastState.quat.y() << "\t" << lastState.quat.z() << endl;

//        std::cout << timestamp
//                  << "," << acc[0]  << "," << acc[1]  << "," << acc[2]
//                  << "," << mag[0]  << "," << mag[1]  << "," << mag[2]
//                  << "," << gyro[0] << "," << gyro[1] << "," << gyro[2] << std::endl;



        // Alwasy use mutex, when changing state
        mMotors.calculateControlResponse(lastState);

        waitPeriod();
    }

    std::cout << "KALMANFILTER: Got signal to terminate" << std::endl;
    std::cout << "KALMANFILTER: Stopping Gx3-communicator..." << std::endl;
    mGX3.stop();
    mGX3.join();
    std::cout << "KALMANFILTER: Gx3-communicator joined" << std::endl;
    std::cout << "KALMANFILTER: Terminating now..." << std::endl;

}

bool KalmanFilter::getState()
{
    ScopedLock scLock(mStateLock);
    return mState;
}
