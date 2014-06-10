//author voidccc
#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "IRun.h"
#include "Declear.h"
#include "BlockingQueue.h"

#include <vector>

using namespace std;

class ThreadPool : public IRun
{
    public:
        ThreadPool();
        void start(int numThreads);
        void addTask(IRun* ptask);
    public:
        void virtual run(void* param);
    private:
        void runInThread();
        BlockingQueue<IRun*> _tasks;
        vector<Thread*> _threads;
};

#endif
