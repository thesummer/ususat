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



#include "gx3communicator.h"
using namespace USU;

#include "messages.h"

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

    if(initSettings.sendCommand(mSerialPort) == false)
        throw std::runtime_error("Setting SamplingSettings failed");
}

void GX3Communicator::run()
{

    mKeepRunning = true;

    // Activate Continuous mode
    SetCountinuousMode setCont(ACC_ANG_MAG_VEC);
    if(setCont.sendCommand(mSerialPort) == false)
        ;/// TODO: Error

    while(mKeepRunning)
    {
        AccAngMag data;
        if(data.readFromSerial(mSerialPort) == false)
            ; /// TODO: Error?

        mQueue.push(data);
    }

    // Stop continuous mode
    setCont.mCommand[3] = 0;
    if(setCont.sendCommand(mSerialPort) == false)
        ; /// TODO: Error?
}


