/*!**************************************************************************
 *  \No COPYRIGHT
 *  \file   template.h
 *  \brief  them template class for queue
 *  \author thangvv
 *  \date   07/11/22
 *
 *  \brief Requirements Covered:
 *  \n 1)   REQ_XXXX_Assignment

 *  \note
 *   Revision History:
 *   Date        Author              Description
 *   ------      --------            --------------
 *   06/17/22    thangvv            Initial version

*****************************************************************************/

#ifndef TEMPLATE_INCLUDED
#define TEMPLATE_INCLUDED

#include <queue>
#include <mutex>
#include <condition_variable>

template <class T>
class WaitSignalQueue
{
protected:
    std::queue<T> _queue;
    mutable std::mutex _mutex;
    std::condition_variable _con;

public:
    WaitSignalQueue() : _queue(), _mutex(), _con()
    {
    }

    // Add an element to the queue.
    void enqueue(T t)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.push(t);
        _con.notify_one();
    }

    T dequeue(void)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        while (_queue.empty())
        {
            // release lock as long as the wait and reaquire it afterwards.
            _con.wait(lock);
        }
        T val = _queue.front();
        _queue.pop();
        return val;
    }

    void clear()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        while(!_queue.empty())
            _queue.pop();
    }

    int queue_size()
    {
        //std::unique_lock<std::mutex> lock(_mutex);
        return _queue.size();
    }

    void clear_queue()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        /* erase the buffer by assigning to empty queue */
        _queue = std::queue<T>();
    }

    virtual bool is_dequeue_timeout(T &pop_item, uint64_t wait_time_ms = 500u)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        const std::chrono::milliseconds duration(wait_time_ms);
        auto now = std::chrono::steady_clock::now();
        if (_queue.empty())
        {
            if (std::cv_status::timeout == _con.wait_until(lock, now + duration))
            {
                return true;
            }
        }
        pop_item = _queue.front();
        _queue.pop();
        return false;
    }
};


#endif // TEMPLATE_INCLUDED
