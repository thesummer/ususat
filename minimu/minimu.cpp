#include "minimu.h"
#include "IMU.h"

MinImu::MinImu(const char *i2cDeviceName)
    :compass(i2cDeviceName), gyro(i2cDeviceName)
{

}

void MinImu::enable()
{
    compass.enable();
    gyro.enable();
}

vector MinImu::readGyro()
{
    // At the full-scale=2000 dps setting, the gyro datasheet says
    // we get 0.07 dps/digit.
    //TODO: check datasheet again
    //      maybe smaller full scale is sufficient.
    const float gyro_scale = 0.07 * 3.14159265 / 180;

    gyro.read();
    IMU::raw_g = int_vector_from_ints(&gyro.g);
    return ( vector_from_ints(&gyro.g)  * gyro_scale);
}

vector MinImu::readAcc()
{
    // LSM303 accelerometer: At 8 g sensitivity, the datasheet says
    // we get 3.9 mg/digit.
    // TODO: double check this figure using the correct datasheet
    //       check which sensitivity is set. +-2 g may be enough
    const float accel_scale = 0.0039;

    compass.readAcc();
    IMU::raw_a = int_vector_from_ints(&compass.a);
    return vector_from_ints(&compass.a) * accel_scale;
}

vector MinImu::readMag()
{
    compass.readMag();
    IMU::raw_m = int_vector_from_ints(&compass.m);

    return IMU::raw_m;
}
