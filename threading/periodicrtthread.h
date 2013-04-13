#ifndef PERIODICRTTHREAD_H
#define PERIODICRTTHREAD_H

#include "RtThread.h"

namespace USU {

class PeriodicRtThread : public RtThread
{
private:
    int mTimerFd;
    unsigned long long mMissedWakeUps; /* type expected by read in waitPeriod() */
    unsigned int mPeriod_us;
    void makeThreadPeriodic(); //TODO: create exception
    void waitPeriod();

public:
    PeriodicRtThread(int priority = 0, unsigned int period_us = 1000000);
    virtual void run() = 0;
};

}

#endif // PERIODICRTTHREAD_H
