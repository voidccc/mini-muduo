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

Thread::Thread(IRun* pRun)
    :_run(pRun)
{ }

void Thread::start()
{
    pthread_t t;
    ::pthread_create(&t, NULL, globalRun, this);
}

void Thread::run()
{
    _run->run(NULL);
}

pid_t Thread::gettid()
{
    return static_cast<pid_t>(::syscall(SYS_gettid));
}

