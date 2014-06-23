//author voidccc
#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "Declear.h"
#include "BlockingQueue.h"
#include "Task.h"
#include "IRun.h"

#include <vector>
using namespace std;

class ThreadPool : public IRun0
{
    public:
        ThreadPool();
        void start(int numThreads);
        void addTask(Task& task);
        virtual void run0();
    private:
        void runInThread();
        BlockingQueue<Task> _tasks;
        vector<Thread*> _threads;
};

#endif
