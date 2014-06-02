//author voidccc

#include <sys/eventfd.h>

#include "EventLoop.h"
#include "Channel.h"
#include "Epoll.h"
#include "TimerQueue.h"
#include "Timestamp.h"

#include <iostream>
using namespace std;

EventLoop::EventLoop()
    :_quit(false)
    ,_pPoller(new Epoll()) // Memory Leak !!!
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

void EventLoop::queueLoop(IRun* pRun, void* param)
{
    Runner r(pRun, param);
    _pendingFunctors.push_back(r);
    //wakeup();
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
    vector<Runner> tempRuns;
    tempRuns.swap(_pendingFunctors);
    vector<Runner>::iterator it;
    for(it = tempRuns.begin(); it != tempRuns.end(); ++it)
    {
        (*it).doRun();
    }
}
int EventLoop::runAt(Timestamp when, IRun* pRun)
{
    return _pTimerQueue->addTimer(pRun, when, 0.0);
}

int EventLoop::runAfter(double delay, IRun* pRun)
{
    return _pTimerQueue->addTimer(pRun, Timestamp::nowAfter(delay), 0.0);
}

int EventLoop::runEvery(double interval, IRun* pRun)
{
    return _pTimerQueue->addTimer(pRun, Timestamp::nowAfter(interval), interval);
}

void EventLoop::cancelTimer(int timerId)
{
    _pTimerQueue->cancelTimer(timerId);
}
