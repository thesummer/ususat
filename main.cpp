
#include "RtThread.h"

#include <iostream>
using namespace std;
using namespace USU;

class MyThread: public RtThread
{
public:
    MyThread(int prio)
        : RtThread(prio)
    {}

    void run()
    {
        cout << "Hello from Thread" << endl;
        struct sched_param param;
        int policy;
        pthread_getschedparam(mId, &policy, &param);

        cout << "policy=" << ((policy == SCHED_FIFO)  ? "SCHED_FIFO" :
                             (policy == SCHED_RR)    ? "SCHED_RR" :
                             (policy == SCHED_OTHER) ? "SCHED_OTHER" :
                                                                "???")
                      << ", priority=" << param.sched_priority << endl;
    }
};


int main()
{
    cout << "Creating thread" << endl;
    MyThread thr(5);
    cout << "Starting thread" << endl;
    thr.start();

    thr.join();
    cout << "Thread joined" << endl;


 return 0;
}
