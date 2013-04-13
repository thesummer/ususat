#ifndef LOCK_H
#define LOCK_H

namespace USU {

#include <pthread.h>

class Lock
{
private:
    // Prevent copying or assignment
    Lock(const Lock& arg);
    Lock& operator=(const Lock& rhs);
    pthread_mutex_t mMutex;

public:
    Lock();
    virtual ~Lock();
    void lock();
    void unlock();
};

class ScopedLock
{
private:
    Lock &mLock;

public:
    ScopedLock(Lock &lock);
    virtual ~ScopedLock();
};

}

#endif // LOCK_H
