//#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>

#include "I2CBus.h"

I2CBus::I2CBus(const char * deviceName)
{
    fd = open(deviceName, O_RDWR);
    if (fd == -1)
    {
        throw posix_error("Failed to open I2C device.");
    }
}

I2CBus::~I2CBus()
{
    close(fd);
}

void I2CBus::addressSet(uint8_t address)
{
    int result = ioctl(fd, I2C_SLAVE, address);
    if (result == -1)
    {
        throw posix_error("Failed to set address.");
    }
}

void I2CBus::writeByte(uint8_t command, uint8_t data)
{
    int result = i2c_smbus_write_byte_data(fd, command, data);
    if (result == -1)
    {
        throw posix_error("Failed to write byte to I2C.");
    }
}

void I2CBus::writeByte(uint8_t data)
{
    int result = i2c_smbus_write_byte(fd, data);
    if (result == -1)
    {
        throw posix_error("Failed to write raw byte to I2C.");
    }
}

uint8_t I2CBus::readByte(uint8_t command)
{
    int result = 0;
    result = i2c_smbus_read_byte_data(fd, command);
    if (result == -1)
    {
//        throw posix_error("Failed to read byte from I2C.");
        return 0;
    }
    return (uint8_t) result;
}

uint8_t I2CBus::readByte()
{
    int result = i2c_smbus_read_byte(fd);
    if (result == -1)
    {
        throw posix_error("Failed to read raw byte from I2C.");
    }
    return result;
}

uint16_t I2CBus::readWord(uint8_t command)
{
    int result = i2c_smbus_read_word_data(fd, command);
    if (result == -1)
    {
        throw posix_error("Failed to read word from I2C.");
    }
    return result;
}

uint16_t I2CBus::readWord()
{
    uint16_t temp;
    int result = read(fd, &temp, sizeof(uint16_t) );
    if(result == -1)
    {
        throw posix_error("Failed to read raw word from I2C");
    }
    return temp;
}

int I2CBus::tryReadByte(uint8_t command)
{
    return i2c_smbus_read_byte_data(fd, command);
}


void I2CBus::readBlock(uint8_t command, uint8_t size, uint8_t * data)
{
    int result = i2c_smbus_read_i2c_block_data(fd, command, size, data);
    if (result != size)
    {
        throw posix_error("Failed to read block from I2C.");
    }
}
