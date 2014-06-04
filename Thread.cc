//author voidccc
#include "Thread.h"
#include "BlockingQueue.h"

#include <unistd.h>
#include <sys/syscall.h>

void* globalRun(void* arg)
{
    ((Thread*)arg)->run();
    return 0;
}

Thread::Thread()
{
    
}

void Thread::run()
{
    while(true)
    {
        IRun* task = (IRun*)_tasks.take();
        task->run(NULL);
    } 
}

void Thread::addTask(IRun* ptask)
{
    _tasks.put(ptask);
}

void Thread::start()
{
    pthread_t t;
    ::pthread_create(&t, NULL, globalRun, this);
}

pid_t Thread::gettid()
{
    return static_cast<pid_t>(::syscall(SYS_gettid));
}

