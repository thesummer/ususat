#include "max127.h"

Max127::Max127(char *i2cdevice)
    :mI2c(i2cdevice)
{
    mI2c.addressSet(I2C_ADDRESS);
}

int16_t Max127::readRaw(uin8_t channel)
{
    // Compile the full control byte by setting the channel bits
    uint16_t rawValue = (mI2c.readWord(CONTROL_BYTE | (channel << SEL0) ));

    // direct casting (int16_t) rawValue >> 4 would be possible, but could
    // produce problems on systems with different endianess
    return ( (int16_t) ( (rawValue & 0xFF00) | (rawValue & 0x00FF ) ) >> 4 );


}

float Max127::readVoltage(unsigned int channel)
{
    // fullscale = +-5V --> resolution = 10V/4096
    const float scaleVoltage = 2.4414063e-3f;
    return (readRaw(channel) * scaleVoltage);
}
