//author voidccc
#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

class MutexLock
{
    public:
        MutexLock()
        {
            pthread_mutex_init(&_mutexid, NULL);
        }
        ~MutexLock()
        {
            pthread_mutex_destroy(&_mutexid);
        }
        void lock()
        {
            pthread_mutex_lock(&_mutexid);
        }
        void unlock()
        {
            pthread_mutex_unlock(&_mutexid);
        }

        pthread_mutex_t* getPthreadMutex()
        {
            return &_mutexid;       
        }
    private:
        pthread_mutex_t _mutexid;
};

class MutexLockGuard
{
    public:
        MutexLockGuard(MutexLock& mutex)
            :_mutex(mutex)
        {
            _mutex.lock();        
        }
        ~MutexLockGuard()
        {
            _mutex.unlock();        
        }
    private:
        MutexLock& _mutex;
};

#endif
