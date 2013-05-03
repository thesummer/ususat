/**
 * @file sharedqueue.h
 *
 * Small wrapper class to make std::queue thread safe in the
 * sense of the single producer, single consumer problem.
 *
 * @author Jan Sommer
 *  Created on: May 2, 2013
 *
 */

#ifndef SHAREDQUEUE_H
#define SHAREDQUEUE_H

#include <queue>
using std::queue;

#include "Lock.h"
#include

namespace USU
{
template <class T>
class SharedQueue
{
public:
    SharedQueue();

    void push(const T& newElement)
    {
        ScopedLock(mLock);
        mQueue.push(newElement);
    }

    void pop()
    {
        ScopedLock (mLock);
        mQueue.pop();
    }

    T& front()
    {
        ScopedLock(mLock);
        return mQueue.front();
    }

private:
    Lock mLock;
    queue<T> mQueue;
};

}

#endif // SHAREDQUEUE_H
