//author voidccc
#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "IRun.h"
#include "Declear.h"

#include <vector>

using namespace std;

class ThreadPool
{
    public:
        ThreadPool(int size);
        void addTask(IRun* ptask);
    private:
        Thread* getRandomThread();
        vector<Thread*> _threads;
        int _size;
        int _index;
};

#endif
