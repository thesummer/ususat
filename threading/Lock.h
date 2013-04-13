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


#include <stdio.h>

inline
Lock::Lock()
{
    int ret;
    if ( (ret = pthread_mutex_init(&mMutex, NULL)) != 0)
    {
        perror("pthread_mutex_init ");
    }
}

inline
Lock::~Lock()
{
    int ret;
    if ( (ret = pthread_mutex_destroy(&mMutex) ) != 0)
    {
        perror("pthread_mutex_destroy ");
    }
}


inline
void Lock::lock()
{
    pthread_mutex_lock(&mMutex);
}

inline
void Lock::unlock()
{
    pthread_mutex_unlock(&mMutex);
}

///////////////////////////////=============================/////////////////////////

class ScopedLock
{
private:
    Lock &mLock;

public:
    ScopedLock(Lock &lock);
    virtual ~ScopedLock();
};

inline
ScopedLock::ScopedLock(Lock &lock)
    :mLock(lock)
{
    mLock.lock();
}

inline
ScopedLock::~ScopedLock()
{
    mLock.unlock();
}

}

#endif // LOCK_H
