//author voidccc

#include <pthread.h>
#include "ThreadPool.h"
#include "Thread.h"
#include <iostream>
#include <sstream>
using namespace std;

ThreadPool::ThreadPool(int size)
    :_size(size)
{
    for(int i = 0 ; i < size; i++)
    {
        Thread* p = new Thread();
        _threads.push_back(p);
        p->start();
    }
}

void ThreadPool::addTask(IRun* ptask)
{
    Thread* pthread = getRandomThread();
    pthread->addTask(ptask);
}

Thread* ThreadPool::getRandomThread()
{
    return _threads[_index++ % _size];
}
