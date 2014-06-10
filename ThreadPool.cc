//author voidccc

#include <pthread.h>
#include "ThreadPool.h"
#include "Thread.h"
#include <iostream>
#include <sstream>
using namespace std;

ThreadPool::ThreadPool() { }

void ThreadPool::start(int numThreads)
{
    _threads.reserve(numThreads);
    for(int i = 0 ; i < numThreads; i++)
    {
        Thread* p = new Thread(this);
        _threads.push_back(p);
        p->start();
    }
}

void ThreadPool::addTask(IRun* ptask)
{
    _tasks.put(ptask);
}

//virtual for Thread
void ThreadPool::run(void* param)
{
    runInThread();
}

void ThreadPool::runInThread()
{
    while(true)
    {
        IRun* task = (IRun*)_tasks.take();
        task->run(NULL);
    }
}
