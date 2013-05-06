#ifndef MAX127_H
#define MAX127_H

#include <stdint.h>

#include "../minimu/I2CBus.h"


const uint8_t I2C_ADDRESS  = 0b00101000;
const uint8_t CONTROL_BYTE = 0b10000110;
const uint8_t SEL0         = 4;

class Max127
{
public:
    Max127(char *i2cdevice);

    int16_t readRaw(uin8_t channel);
    float readVoltage(unsigned int channel);

private:
    I2CBus mI2c;


};

#endif // MAX127_H
