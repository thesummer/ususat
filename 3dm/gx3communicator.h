/**
 * @file gx3communicator.h
 *
 * Contains the thread which handles the communication to the
 * 3DM-GX3-25.
 *
 * @author Jan Sommer
 *  Created on: Apr 26, 2013
 *
 */

#ifndef GX3COMMUNICATOR_H
#define GX3COMMUNICATOR_H

#include<SerialPort.h>
#include "../threading/RtThread.h"

namespace USU
{

/*!
 \brief Represents the Thread class for communication with the 3DM-GX3-25

 The class is derived from RtThread.
 It initializes the serial interface to the 3DM and sets the sampling settings.
 Finally it starts the continuous mode and polls the serial port for new arrived data.
 If new data arrived it is presented for the KalmanFilter to take and consider.

*/
class GX3Communicator : public RtThread
{
public:
    /*!
     \brief Constructor of the class

     Sets up the serial port and thread attributes.

     \param priority  Priority of the pthread (1..99)
     \param serialDevice Name of the serial device
     \param baudRate Baud rate for the serial device (if different from 115200)
     */
    GX3Communicator(int priority, const char* serialDevice,
                    SerialPort::BaudRate baudRate = SerialPort::BAUD_115200);

    /*!
     \brief Thread routine

        - Set sampling settings of 3DM
        - Start continuous mode
        - Poll serial port for newly arrived packages
        - Convert binary data
        - TODO: Send new package to KalmanFilter
    */
    virtual void run();


    /*!
     \brief Signals the thread to stop
    */
    void stop() {mKeepRunning = false;}

private:
    GX3Communicator(const GX3Communicator& thread); /*!< Copy constructor made inaccessible by declaring it private */

    GX3Communicator& operator=(const GX3Communicator& rhs); /*!< Assignment constructor made inaccessible by declaring it private */

    SerialPort mSerialPort; /*!< Handles the serial port communication */
    volatile bool mKeepRunning;  /*!< Indicates if the Thread should keep running. volatile to prevent optimizing */
};

}

#endif // GX3COMMUNICATOR_H
