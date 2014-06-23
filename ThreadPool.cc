//author voidccc

#include "ThreadPool.h"
#include "Thread.h"

ThreadPool::ThreadPool() { }

void ThreadPool::start(int numThreads)
{
    _threads.reserve(numThreads);
    for(int i = 0 ; i < numThreads; i++)
    {
        Task task(this);
        Thread* p = new Thread(task);
        _threads.push_back(p);
        p->start();
    }
}

//virtual for Thread
void ThreadPool::addTask(Task& task)
{
    _tasks.put(task);
}

//virtual for Thread class
void ThreadPool::run0()
{
    runInThread();
}

void ThreadPool::runInThread()
{
    while(true)
    {
        _tasks.take().doTask();
    }
}
