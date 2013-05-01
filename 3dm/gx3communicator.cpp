/**
 * @file gx3communicator.cpp
 *
 * Contains the thread which handles the communication to the
 * 3DM-GX3-25.
 *
 * @author Jan Sommer
 *  Created on: Apr 26, 2013
 *
 */

#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <stdexcept>

#include <sys/time.h>


#include "gx3communicator.h"
using namespace USU;

#include "messages.h"

int timeval_subtract (struct timeval * result, struct timeval * x, struct timeval * y)
{
    /* Perform the carry for the later subtraction by updating y. */
    if (x->tv_usec < y->tv_usec) {
        int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
        y->tv_usec -= 1000000 * nsec;
        y->tv_sec += nsec;
    }
    if (x->tv_usec - y->tv_usec > 1000000) {
        int nsec = (x->tv_usec - y->tv_usec) / 1000000;
        y->tv_usec += 1000000 * nsec;
        y->tv_sec -= nsec;
    }

    /* Compute the time remaining to wait.
          tv_usec is certainly positive. */
    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_usec = x->tv_usec - y->tv_usec;

    /* Return 1 if result is negative. */
    return x->tv_sec < y->tv_sec;
}

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

    // Activate Continuous mode
    SetCountinuousMode setCont(ACC_ANG_MAG_VEC);
    mSerialPort.WriteRaw(setCont.mCommand, SetCountinuousMode::size);

    struct timeval start, now, elapsed;
    gettimeofday(&start, NULL);

    while(mKeepRunning)
    {
        uint8_t buffer[RawAccAng::size];

//        mSerialPort.WriteByte(ACC_ANG);

        buffer[0] = mSerialPort.ReadByte();
        if(buffer[0] == ACC_ANG_MAG_VEC)
        {
            mSerialPort.ReadRaw(&buffer[1], AccAngMag::size-1);
            if(GX3Packet::calculateChecksum(buffer, AccAngMag::size))
            {
                AccAngMag test(buffer);
                gettimeofday(&now, NULL);
                timeval_subtract(&elapsed, &now, &start);
                unsigned long long timestamp = elapsed.tv_sec * 1000 + elapsed.tv_usec / 1000; // in ms since start
                std::cout << test.timer/62 << "," << timestamp << ","
                          << test.acc[0] << "," << test.acc[1] << "," << test.acc[2] << ","
                          << test.gyro[0]<< "," << test.gyro[1]<< "," << test.gyro[2]<< ","
                          << test.mag[0] << "," << test.mag[1] << "," << test.mag[2] << std::endl;
            }
        }
//        usleep(100000);
    }
}


