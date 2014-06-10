//author voidccc
#ifndef THREAD_H
#define THREAD_H

#include "BlockingQueue.h"
#include "IRun.h"

#include <pthread.h>

class Thread
{
    public:
        Thread(IRun* pRun);
        void start();
        void run();
        pid_t gettid();
    private:
        IRun* _run;
};

#endif
