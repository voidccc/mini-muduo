//author voidccc
#include "Thread.h"

#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>

namespace CurrentThread
{
  __thread int t_cachedTid = 0;
}

void* globalRun(void* arg)
{
    ((Task*)arg)->doTask();
    return 0;
}

Thread::Thread(Task& task)
    :_task(task)
{ }

void Thread::start()
{
    pthread_t t;
    ::pthread_create(&t, NULL, globalRun, &_task);
}

pid_t Thread::gettid()
{
    return static_cast<pid_t>(::syscall(SYS_gettid));
}

