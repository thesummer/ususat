#ifndef MINIMU_H
#define MINIMU_H

#include "LSM303.h"
#include "L3G.h"

class MinImu : public IMU
{
public:
    LSM303 compass;
    L3G gyro;

    MinImu(const char * i2cDeviceName);
    virtual vector readMag();  // In body coords, currently without scaling
    virtual vector readAcc();  // In body coords, with units = g
    virtual vector readGyro(); // In body coords, with units = rad/sec

    virtual void enable();
};

#endif // MINIMU_H
