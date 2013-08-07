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
      mGX3(priority, "/dev/ttyO4"), mKeepRunning(false)
{
}

void KalmanFilter::run()
{
    /// TODO: make different modes
    vector acc, mag, gyro;
    mKeepRunning = true;
    struct timeval start, now, elapsed;

    mImu.enable();

    std::vector<Command>::const_iterator commandIt = mCommandList.begin();
    mMotors.setSetValue(commandIt->angVel);
    int countdown = commandIt->time;


    gettimeofday(&start, NULL);
    unsigned lastTime = 0;
    waitPeriod();

    while(mKeepRunning)
    {
        gettimeofday(&now, NULL);

        //Only use gyro at first
//        acc  = mImu.readAcc();
//        mag  = mImu.readMag();
        gyro = mImu.readGyro();

        timeval_subtract(&elapsed, &now, &start);
        unsigned time = elapsed.tv_sec * 1000 + elapsed.tv_usec / 1000; // in ms since start
        countdown -= time - lasttime;
        lastTime = time;
        if(countdown <=0)
        {
            commandIt++;
            // if at the end of the commandList start again from the beginning
            if(commandIt == mCommandList.end())
                commandIt = mCommandList.begin();

            countdown = commandIt->time;
            mMotors.setSetValue(commandIt->angVel);
        }

        // Alwasy use mutex, when changing state
         mMotors.controlFromGyro(gyro);

        waitPeriod();
    }

    std::cerr << "KALMANFILTER: Got signal to terminate" << std::endl;
    std::cerr << "KALMANFILTER: Stopping Gx3-communicator..." << std::endl;
    mGX3.stop();
    if(mGX3.join() )
    {
        std::cerr << "KALMANFILTER: Gx3-communicator joined" << std::endl;
    }
    else
    {
        std::cerr << "KALMANFILTER: Joining Gx3-communicator failed" << std::endl;
    }
    std::cerr << "KALMANFILTER: Terminating now..." << std::endl;

}

bool KalmanFilter::getState()
{
    ScopedLock scLock(mStateLock);
    return mState;
}

void KalmanFilter::initializeModeSimpleControl(std::string trajFilename, float pgain)
{
    std::ifstream inFile;
    inFile.open(filename);
    if(!inFile.is_open())
        throw std::runtime_error("MotorController: Could not open input file");

    Command temp;
    while(true)
    {
        file >> temp.time;

        float x,y,z;
        file >> x >> y >> z;
        temp.angVel << x, y, z;

        if(file.eof())
            break;
        mCommandList.push_back(temp);
    }
    file.close();

    mMotors.setPGain(pgain);
}
