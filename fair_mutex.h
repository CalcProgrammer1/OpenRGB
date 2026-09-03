/*---------------------------------------------------------*\
| fair_mutex.h                                              |
|                                                           |
|   Fair (FIFO) mutex for serialising I2C/SMBus transfers   |
|   Services callers in the order they lock the mutex       |
|                                                           |
|   Adam Honse (CalcProgrammer1)                02 Sep 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <mutex>
#include <condition_variable>

/*---------------------------------------------------------*\
| fair_mutex                                                |
|                                                           |
| A fair (FIFO) mutex that services lock requests in the    |
| order they are received, preventing any single caller     |
| from monopolising the mutex at the expense of callers     |
| that have been waiting longer.                            |
|                                                           |
| This is implemented as a ticket lock: every caller draws  |
| a sequentially-increasing ticket number and then waits    |
| until that ticket is at the head of the queue.  An        |
| internal std::mutex guards the ticket counters; a         |
| std::condition_variable wakes all waiters when a ticket   |
| reaches the front of the queue.                           |
|                                                           |
| The class satisfies the BasicLockable requirements        |
| (lock / unlock) so it can be used with std::lock_guard    |
| and std::unique_lock, or with direct lock() / unlock()    |
| calls.                                                    |
\*---------------------------------------------------------*/
class fair_mutex
{
public:
    fair_mutex()  = default;
    ~fair_mutex() = default;

    fair_mutex(const fair_mutex&)            = delete;
    fair_mutex& operator=(const fair_mutex&) = delete;

    /*-----------------------------------------------------*\
    | Acquire the mutex.                                    |
    |                                                       |
    | Draws the next ticket and blocks until that ticket    |
    | becomes the one being served, guaranteeing FIFO       |
    | ordering among all waiting threads.                   |
    \*-----------------------------------------------------*/
    void lock()
    {
        std::unique_lock<std::mutex> lock(internal_mutex);

        const unsigned int ticket = next_ticket++;

        while(ticket != serving)
        {
            cv.wait(lock);
        }
    }

    /*-----------------------------------------------------*\
    | Release the mutex.                                    |
    |                                                       |
    | Increments the "serving" counter so that the next     |
    | ticket in the queue becomes eligible to proceed, then |
    | wakes all waiting threads.                            |
    \*-----------------------------------------------------*/
    void unlock()
    {
        std::lock_guard<std::mutex> lock(internal_mutex);

        serving++;
        cv.notify_all();
    }

private:
    /*-----------------------------------------------------*\
    | Guards the ticket counters.  This is only held for    |
    | very brief critical sections (incrementing the        |
    | counters) so it does not interfere with the fairness  |
    | of the outer mutex.                                   |
    \*-----------------------------------------------------*/
    std::mutex              internal_mutex;

    /*-----------------------------------------------------*\
    | Sleeps waiters until their ticket reaches the front   |
    | of the queue.  notify_all() is used so that the       |
    | correct waiter always wakes up regardless of the      |
    | implementation's condition variable scheduling.       |
    \*-----------------------------------------------------*/
    std::condition_variable cv;

    /*-----------------------------------------------------*\
    | Sequentially-assigned ticket number.  Incremented     |
    | atomically (under internal_mutex) each time a thread  |
    | calls lock().                                         |
    |                                                       |
    | Wrapped unsigned arithmetic is safe here: the FIFO    |
    | property is preserved even on wrap-around, and the    |
    | probability of approaching 2^32 acquisitions is       |
    | negligible for this use case.                         |
    \*-----------------------------------------------------*/
    unsigned int            next_ticket = 0;

    /*-----------------------------------------------------*\
    | The ticket number currently being served.  A waiter   |
    | whose ticket equals this value is permitted to        |
    | proceed.                                              |
    \*-----------------------------------------------------*/
    unsigned int            serving     = 0;
};
