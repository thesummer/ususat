#include "kalmanfilter.h"
#include "minimu/vector.h"
using namespace USU;

KalmanFilter::KalmanFilter(char *i2cBus, USU::SharedState &state)
    :mImu(i2cBus), mState(state), mKeepRunning(false)
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

        waitPeriod();
    }

}

bool KalmanFilter::getState() const
{
    ScopedLock scLock(mStateLock);
    return mState;
}
