/**
 * @file RtThread.cpp
 *
 * Small C++ wrapper class to create a realtime scheduled pthread
 *
 * @author Jan Sommer
 *  Created on: Apr 10, 2013
 *
 */

#include "RtThread.h"
using namespace USU;

#include <stdio.h>


RtThread::RtThread(int priority):
    mPriority(priority), mStarted(false), mId(-1)
{
    int ret;
    if ( (ret = pthread_attr_init(&mAttr)) != 0)
    {
        perror("phtread_attr_init ");
        throw "Error";
    }
    // Set scheduler to (realtime) FIFO
    if ( (ret = pthread_attr_setschedpolicy(&mAttr, SCHED_FIFO)) != 0)
    {
        perror("pthread_attr_setschedpolicy");
        throw "Error";
    }

    // Change priority for the thread to mPriority
    struct sched_param param;
    if ( (ret = pthread_attr_getschedparam(&mAttr, &param)) != 0)
    {
        perror("pthread_attr_getschedparam");
        throw "Error";
    }
    param.__sched_priority = mPriority;

    if ( (ret = pthread_attr_setschedparam(&mAttr, &param)) != 0)
    {
        perror("pthread_attr_setschedparam");
        throw "Error";
    }

    /*
     * Set inherit scheduler attribut to PTHREAD_EXPLICIT_SCHED
     * otherwise the schedule attributes in mAttr will be ignored
     * and the same settings as the main thread will be inherited.
     */
    if ( (ret = pthread_attr_setinheritsched(&mAttr, PTHREAD_EXPLICIT_SCHED)) != 0)
    {
        perror("pthread_attr_setinheritsched ");
        throw "Error";
    }
}

RtThread::~RtThread()
{
    /*
     * Make sure that the thread terminated properly
     * before deleting the instance
     */
    this->join();
    int ret;
    if ( (ret = pthread_attr_destroy(&mAttr)) != 0)
    {
        perror("pthread_attr_destroy");
        throw "Error";
    }
}

inline
pthread_t RtThread::getThreadId() const
{
    return mId;
}

inline
int RtThread::getPriority() const
{
    return mPriority;
}

void RtThread::start(void *arg)
{
    int ret;
    mArgs = arg;
    /*
    * Since pthread_create is a C library function, the 3rd argument is
    * a global function that will be executed by the thread. In C++, we
    * emulate the global function using the static member function that
    * is called exec. The 4th argument is the actual argument passed to
    * the function exec. Here we use this pointer, which is an instance
    * of the Thread class.
    */

    if ( (ret = pthread_create(&mId, &mAttr, &RtThread::exec, this)) !=0)
    {
        perror("thread_create ");
        throw "Error";
    }
    mStarted = true;
}

void RtThread::join()
{
    //Allow the thread to wait for the termination status
    if (mStarted)
    {
        pthread_join(mId, NULL);
    }
}

// Function which is actually executed by the thread
void * RtThread::exec(void *thr)
{
    reinterpret_cast<RtThread *> (thr)->run();
    return NULL;
}


