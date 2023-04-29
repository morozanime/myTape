#ifndef ASYNC_QUEUE_H
#define ASYNC_QUEUE_H

#include <QThread>
#include <QQueue>
#include <QMutex>

template<class T> class QAsyncQueue
{
    public:

        QAsyncQueue(uint max = -1)
            : _max(max)
        {
        }

        ~QAsyncQueue()
        {
            clear();
        }

        uint count()
        {
            _mutex.lock();
            int count = _queue.count();
            _mutex.unlock();
            return count;
        }

        bool isFull()
        {
            if (-1 == _max)
                return false;

            _mutex.lock();
            int count = _queue.count();
            _mutex.unlock();
            return count >= _max;
        }

        bool isEmpty()
        {
            _mutex.lock();
            bool empty = _queue.isEmpty();
            _mutex.unlock();
            return empty;
        }

        void clear()
        {
            _mutex.lock();
            _queue.clear();
            _mutex.unlock();
        }

        void enqueue(const T& t)
        {
            _mutex.lock();
            _queue.enqueue(t);
            _mutex.unlock();
        }

        T dequeue()
        {
            _mutex.lock();
            T i = _queue.dequeue();
            _mutex.unlock();
            return i;
        }

        T first()
        {
            _mutex.lock();
            T i = _queue.first();
            _mutex.unlock();
            return i;
        }

    private:

        QQueue<T> _queue;
        QMutex _mutex;
        int _max;
};

#endif // ASYNC_QUEUE_H
