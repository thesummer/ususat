#ifndef GX3COMMUNICATOR_H
#define GX3COMMUNICATOR_H

#include<SerialPort.h>
#include "../threading/RtThread.h"

namespace USU
{

class GX3Communicator : public RtThread
{
public:
    GX3Communicator(int priority, const char* serialDevice,
                    SerialPort::BaudRate baudRate = SerialPort::BAUD_115200);

    virtual void run();

    void stop() {mKeepRunning = false;}

private:
    GX3Communicator(const GX3Communicator& thread); /*!< Copy constructor made inaccessible by declaring it private */
    GX3Communicator& operator=(const GX3Communicator& rhs); /*!< Assignment constructor made inaccessible by declaring it private */

    SerialPort mSerialPort;
    volatile bool mKeepRunning;
};

}

#endif // GX3COMMUNICATOR_H
