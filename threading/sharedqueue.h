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
/*!
 \brief Wrapper class to make std::queue thread safe

 Protects the push, pop and front access from thread using
 a mutex.
 It can only handle one reader and one writer thread at a time.
 Multiple reader threads could produce race conditions!!!

*/
class SharedQueue
{
public:
    /*!
     \brief Constructor, creates an empty queue

    */
    SharedQueue();

    /*!
     \brief Adds a new element to the back of the queue

     Takes the mutex before the write operation.

     \param newElement the element to be added
    */
    void push(const T& newElement)
    {
        ScopedLock(mLock);
        mQueue.push(newElement);
    }

    /*!
     \brief Destroys the first (oldest) element in the queue

     Takes mutex before the write operation.
     Calls the destroy operator of the current front-element.

    */
    void pop()
    {
        ScopedLock (mLock);
        mQueue.pop();
    }

    /*!
     \brief Returns a reference to the first (oldest) element in the queue.

     Takes a mutex before accesing the first element.

     \return T
    */
    T& front()
    {
        ScopedLock(mLock);
        return mQueue.front();
    }

private:
    Lock mLock; /*!< Mutex to protect the queue */
    queue<T> mQueue; /*!< Standard queue to store the data */
};

}

#endif // SHAREDQUEUE_H
