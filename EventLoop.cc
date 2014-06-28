//author voidccc

#include <sys/eventfd.h>

#include "EventLoop.h"
#include "Channel.h"
#include "Epoll.h"
#include "TimerQueue.h"
#include "Timestamp.h"
#include "Task.h"
#include "CurrentThread.h"

#include <iostream>
using namespace std;

EventLoop::EventLoop()
    :_quit(false)
    ,_callingPendingFunctors(false)
    ,_pPoller(new Epoll()) // Memory Leak !!!
    ,_threadId(CurrentThread::tid())
    ,_pTimerQueue(new TimerQueue(this)) // Memory Leak!!!
{
    _eventfd = createEventfd();
    _pEventfdChannel = new Channel(this, _eventfd); // Memory Leak !!!
    _pEventfdChannel->setCallback(this);
    _pEventfdChannel->enableReading();
}

EventLoop::~EventLoop()
{}

void EventLoop::loop()
{
    while(!_quit)
    {
        vector<Channel*> channels;
        _pPoller->poll(&channels);

        vector<Channel*>::iterator it;
        for(it = channels.begin(); it != channels.end(); ++it)
        {
            (*it)->handleEvent(); 
        }

        doPendingFunctors();
    }
}

void EventLoop::update(Channel* pChannel)
{
    _pPoller->update(pChannel);
}

void EventLoop::queueInLoop(Task& task)
{
    {
        MutexLockGuard guard(_mutex);
        _pendingFunctors.push_back(task);
    }

    if(!isInLoopThread() || _callingPendingFunctors)
    {
        wakeup();
    }
}

void EventLoop::runInLoop(Task& task)
{
    if(isInLoopThread())
    {
        task.doTask();
    }
    else
    {
        queueInLoop(task);
    }
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = ::write(_eventfd, &one, sizeof one);
    if (n != sizeof one)
    {
        cout << "EventLoop::wakeup() writes " << n << " bytes instead of 8" << endl;
    }
}

int EventLoop::createEventfd()
{
   int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
   if (evtfd < 0)
   {
       cout << "Failed in eventfd" << endl;
   }
   return evtfd;
}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    ssize_t n = ::read(_eventfd, &one, sizeof one);
    if (n != sizeof one)
    {
        cout << "EventEventLoop::handleRead() reads " << n << " bytes instead of 8" << endl;
    }
}

void EventLoop::handleWrite()
{}

void EventLoop::doPendingFunctors()
{
    vector<Task> tempRuns;
    _callingPendingFunctors = true;
    {
        MutexLockGuard guard(_mutex);
        tempRuns.swap(_pendingFunctors);
    }
    vector<Task>::iterator it;
    for(it = tempRuns.begin(); it != tempRuns.end(); ++it)
    {
        it->doTask();
    }
    _callingPendingFunctors = false;
}
int EventLoop::runAt(Timestamp when, IRun0* pRun)
{
    return _pTimerQueue->addTimer(pRun, when, 0.0);
}

int EventLoop::runAfter(double delay, IRun0* pRun)
{
    return _pTimerQueue->addTimer(pRun, Timestamp::nowAfter(delay), 0.0);
}

int EventLoop::runEvery(double interval, IRun0* pRun)
{
    return _pTimerQueue->addTimer(pRun, Timestamp::nowAfter(interval), interval);
}

void EventLoop::cancelTimer(int timerId)
{
    _pTimerQueue->cancelTimer(timerId);
}

bool EventLoop::isInLoopThread()
{
    return _threadId == CurrentThread::tid();
}
