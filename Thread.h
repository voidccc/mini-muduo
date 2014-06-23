//author voidccc
#ifndef THREAD_H
#define THREAD_H

#include "Declear.h"
#include "Task.h"

class Thread
{
    public:
        Thread(Task& task);
        void start();
        pid_t gettid();
    private:
        Task _task;
};

#endif
