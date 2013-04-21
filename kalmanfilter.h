#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include "threading/periodicrtthread.h"
#include "minimu/minimu.h"
#include "threading/Lock.h"

namespace USU
{

class KalmanFilter : public PeriodicRtThread
{
public:
    KalmanFilter(char* i2cBus, SharedState& state);

    virtual void run();

    void stop() { mKeepRunning = false; }

    bool getState() const;

private:
    MinImu mImu;
    ///TODO: semaphore for 3DM
    ///TODO: semaphore to ethernet
    bool mState; ///TODO: Actual representation of the state estimate
    Lock mStateLock;
    volatile bool mKeepRunning;
};

}

#endif // KALMANFILTER_H
