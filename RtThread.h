#ifndef RTTHREAD_H
#define RTTHREAD_H

#include<pthread.h>

namespace USU {

class RtThread
{

private:
    pthread_attr_t mAttr;
    int mPriority;
    pthread_t mId;

    // Prevent copying or assignment
    RtThread(const RtThread& thread);
    RtThread& operator=(const RtThread& rhs);

protected:
    bool mStarted;
    void *mArgs;
    static void *exec(void * thr);

public:
    RtThread(int priority = 0);
    virtual ~RtThread();
    pthread_t getThreadId() const;
    int getPriority() const;
    void start(void * args = NULL);
    void join();
    virtual void run() = 0;
};

}

#endif // RTTHREAD_H
