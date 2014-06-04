//author voidccc
#ifndef THREAD_H
#define THREAD_H

#include "BlockingQueue.h"
#include "IRun.h"

#include <pthread.h>

class Thread
{
    public:
        Thread();
        void start();
        void addTask(IRun* ptask);
        void run();
        pid_t gettid();
    private:
        BlockingQueue<IRun*> _tasks;
};

#endif
