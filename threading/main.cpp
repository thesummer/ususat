
#include "periodicrtthread.h"

#include <iostream>
using namespace std;
using namespace USU;

class MyThread: public PeriodicRtThread
{
public:
    MyThread(int prio)
        : PeriodicRtThread(prio, 1000000)
    {}

    void run()
    {
        cout << "Hello from Thread" << endl;
        for(int i = 0; i<10; i++)
        {
            struct sched_param param;
            int policy;
            pthread_getschedparam(mId, &policy, &param);

            cout << "policy=" <<  ((policy == SCHED_FIFO)  ? "SCHED_FIFO" :
                                  (policy == SCHED_RR)    ? "SCHED_RR" :
                                  (policy == SCHED_OTHER) ? "SCHED_OTHER" :
                                                                                                                "???")
                 << ", priority=" << param.sched_priority << endl;
            waitPeriod();
        }
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
