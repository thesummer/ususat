#ifndef MOTOR_PROGRAMMER_H
#define MOTOR_PROGRAMMER_H

#include <time.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <cerrno>

#include <queue>
#include <fstream>
#include <iostream>
using std::cout;
using std::endl;

#include "RtThread.h"
#include "motorcontrol.h"
#include "datacollector.hpp"
using namespace USU;

class MotorProgrammer: public RtThread
{
public:
    struct Command
    {
        unsigned int time;
        unsigned int motor;
        int dutyCycle;
    };

    MotorProgrammer(int priority, const char* inputFile, const char *outputFile, unsigned int period_us);

    virtual void run();

    volatile bool mKeepRunning;

private:
    int mTimerFd;
    struct timespec mStartTime;
    std::queue<Command> mCommandQueue;
    MotorControl mMotors;
    DataCollector mDataCollector;

};


MotorProgrammer::MotorProgrammer(int priority, const char *inputFile,
                                 const char* outputFile, unsigned int period_us)
    :RtThread(priority), mDataCollector(priority-1, period_us, outputFile, mMotors)
{
    // Initialize the timer
    mTimerFd = timerfd_create (CLOCK_MONOTONIC, 0);
    if ( mTimerFd == -1)
    {
        perror("timer_create ");
        return;
    }


    // Read input file
    std::fstream file;
    file.open(inputFile, std::ios_base::in);

    Command temp;
    while(true)
    {
        file >> temp.time;
        file >> temp.motor;
        file >> temp.dutyCycle;
        if(file.eof())
            break;
        mCommandQueue.push(temp);
    }

    file.close();

    cout << "Read " << mCommandQueue.size() << " commands." << endl;
}

void MotorProgrammer::run()
{
    if (clock_gettime(CLOCK_MONOTONIC, &mStartTime) == -1)
    {
        perror("clock_gettime ");
        return;
    }


    cout << "Starting data collector " << endl;
    mDataCollector.start();


    mKeepRunning = true;
    while(mKeepRunning && !mCommandQueue.empty())
    {

        Command nextEvent = mCommandQueue.front();

        struct itimerspec nextTimer;
        // single event (no period)
        nextTimer.it_interval.tv_sec  = 0;
        nextTimer.it_interval.tv_nsec = 0;

        // next alarm in ms from mStartTime
        int sec  = nextEvent.time /1000;
        int nsec = (nextEvent.time - sec*1000) * 1000000;

        nextTimer.it_value.tv_sec  = mStartTime.tv_sec  + sec;
        nextTimer.it_value.tv_nsec = mStartTime.tv_nsec + nsec;

        // check for overflow
        if (nextTimer.it_value.tv_nsec >= 1000000000)
        {
            nextTimer.it_value.tv_sec++;
            nextTimer.it_value.tv_nsec -= 1000000000;
        }

        int ret;
        // start timer
        if ( (ret = timerfd_settime(mTimerFd, TFD_TIMER_ABSTIME, &nextTimer, NULL)) != 0)
        {
            perror("timerfd_settime ");
            return;
        }

        // wait for timer
        unsigned long long buffer;
        if ( (ret = read(mTimerFd, &buffer, sizeof(buffer) )) == -1 )
        {
            perror ("timer read ");
            return;
        }

        // Do command
        mMotors.setMotor(nextEvent.motor, nextEvent.dutyCycle);
        mCommandQueue.pop();

        cout << "Motor: " << nextEvent.motor
             << "\tDutyCycle: " << nextEvent.dutyCycle << endl;

        // Do subsequent commands (if any)
        while(nextEvent.time == mCommandQueue.front().time)
        {
            nextEvent = mCommandQueue.front();
            mMotors.setMotor(nextEvent.motor, nextEvent.dutyCycle);
            mCommandQueue.pop();
            cout << "Motor: " << nextEvent.motor
                 << "\tDutyCycle: " << nextEvent.dutyCycle << endl;
        }
    }

    cout << "Finished commandlist" << endl;
    cout << "Stopping data collector" << endl;
    mDataCollector.mKeepRunning = false;
    mDataCollector.join();
    cout << "Data collector joined" << endl;
    cout << "Terminating" << endl;
}

#endif
