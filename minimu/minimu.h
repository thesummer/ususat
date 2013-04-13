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
    virtual vector readMag() = 0;  // In body coords, currently without scaling
    virtual vector readAcc() = 0;  // In body coords, with units = g
    virtual vector readGyro() = 0; // In body coords, with units = rad/sec

    virtual void enable();
};

#endif // MINIMU_H
