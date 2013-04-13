#include "periodicrtthread.h"
using namespace USU;
#include <iostream>
using namespace std;
#include <string.h>

#include<sys/timerfd.h>

PeriodicRtThread::PeriodicRtThread(int priority, unsigned int period_us)
    :RtThread(priority), mMissedWakeUps(0), mPeriod_us(period_us)
{
    makeThreadPeriodic();
}


void PeriodicRtThread::makeThreadPeriodic()
{
    int ret;
    unsigned int ns;
    unsigned int sec;
    int fd;
    struct itimerspec itval;

    /* Create the timer */
    if (mTimerFd = timerfd_create (CLOCK_MONOTONIC, 0) != 0)
    {
        cout << "timerfd_create " << strerror(fd) << endl;
        return;
    }

    /* Make the timer periodic */
    sec = mPeriod_us/1000000;
    ns = (mPeriod_us - (sec * 1000000)) * 1000;
    itval.it_interval.tv_sec = sec;
    itval.it_interval.tv_nsec = ns;
    itval.it_value.tv_sec = sec;
    itval.it_value.tv_nsec = ns;
    ret = timerfd_settime (mTimerFd, 0, &itval, NULL);
}

void PeriodicRtThread::waitPeriod()
{
    unsigned long long missed;
    int ret;

    /* Wait for the next timer event. If we have missed any the
           number is written to "missed" */
    ret = read (mTimerFd, &missed, sizeof (missed));
    if (ret == -1)
    {
        cout << "read " << strerror(ret) << endl;
        return;
    }

    /* "missed" should always be >= 1, but just to be sure, check it is not 0 anyway */
    if (missed > 0)
        mMissedWakeUps += (missed - 1);
}
