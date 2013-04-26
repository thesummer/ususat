#include "gx3communicator.h"
using namespace USU;

#include "messages.h"
#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <stdexcept>

GX3Communicator::GX3Communicator(int priority, const char *serialDevice, SerialPort::BaudRate baudRate)
    :RtThread(priority), mSerialPort(serialDevice), mKeepRunning(false)
{
    mSerialPort.Open(baudRate);
    if(mSerialPort.IsOpen() == false)
        throw std::runtime_error("Opening SerialPort failed");

    /*
       Set up the 3DM-GX25 with the following settings (different from default):
        - Data rate 50 Hz
        - Enable little endian for floating points
     */
    SamplingSettings initSettings(SamplingSettings::Change, 20,
                                  SamplingSettings::FlagDefault | SamplingSettings::FlagFloatLittleEndian);
    mSerialPort.WriteRaw(initSettings.mCommand, SamplingSettings::size);

    uint8_t buffer[SamplingSettings::responseSize];
    buffer[0] = mSerialPort.ReadByte();
    if(buffer[0] != SAMPLING_SETTINGS)
        throw std::runtime_error("Setting sampling settings returned unknown response");

    mSerialPort.ReadRaw(&buffer[1], SamplingSettings::responseSize-1);
    if (initSettings.checkResponse(buffer, SamplingSettings::responseSize) == false)
        throw std::runtime_error("Checksum of sampling settings response wrong");
}

void GX3Communicator::run()
{

    mKeepRunning = true;

    while(mKeepRunning)
    {
        uint8_t buffer[RawAccAng::size];

        mSerialPort.WriteByte(ACC_ANG);

        buffer[0] = mSerialPort.ReadByte();
        if(buffer[0] == ACC_ANG)
        {
            mSerialPort.ReadRaw(&buffer[1], RawAccAng::size-1);
            if(GX3Packet::calculateChecksum(buffer, RawAccAng::size))
            {
                RawAccAng test(buffer);
                std::cout << test.acc.transpose() << "\t" << test.gyro.transpose() << std::endl;
            }
        }
        usleep(100000);
    }
}
