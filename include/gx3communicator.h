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
#include<memory>
#include "RtThread.h"
#include "sharedqueue.h"
#include "messages.h"

namespace USU
{

/*!
 \brief Represents the Thread class for communication with the 3DM-GX3-25

 The class is derived from RtThread.
 It initializes the serial interface to the 3DM and sets the sampling settings.
 Finally it starts the continuous mode and polls the serial port for new arrived data.
 New data is stored in a FIFO queue.

 TODO: Use the parent class for the package instead to make it more generic.

 \ingroup 3dm

*/

/*!
 \brief Shared pointer for packages

 In order to store any kind of a GX3Package in the queue a pointer must be used.
 Shared pointer is used to avoid memory leaks.
*/
typedef std::shared_ptr<GX3Packet> packet_ptr;

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
     \brief Initialize the SerialPort and the MicroStrain IMU
    */
    void initialize();

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

    /*!
     \brief Delete the first element of the FIFO.
    */
    void pop() { mQueue.pop();}


    /*!
     \brief Check if the FIFO is empty

     \return bool true, if empty
    */
    bool isEmpty() {return mQueue.isEmpty(); }

    /*!
     \brief Return the number of elements in the FIFO

     \return unsigned number of elements
    */
    unsigned size() {return mQueue.size(); }

    /*!
     \brief Return the first element from the FIFO

     TODO: Make a blocking version of it

     \return AccAngMag the first element
    */
    packet_ptr &front() { return mQueue.front(); }

private:
    GX3Communicator(const GX3Communicator& thread); /*!< Copy constructor made inaccessible by declaring it private */

    GX3Communicator& operator=(const GX3Communicator& rhs); /*!< Assignment constructor made inaccessible by declaring it private */

    SerialPort mSerialPort; /*!< Handles the serial port communication */
    SharedQueue<packet_ptr> mQueue;
    SerialPort::BaudRate mBaudRate;

    volatile bool mKeepRunning;  /*!< Indicates if the Thread should keep running. volatile to prevent optimizing */
};

}

#endif // GX3COMMUNICATOR_H
