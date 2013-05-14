//author voidccc

#include <sys/eventfd.h>

#include "EventLoop.h"
#include "Channel.h"
#include "Epoll.h"

#include <iostream>
using namespace std;

EventLoop::EventLoop()
    :_quit(false)
    ,_pPoller(new Epoll()) // Memory Leak !!!
{
    _eventfd = createEventfd();
    _wakeupChannel = new Channel(this, _eventfd); // Memory Leak !!!
    _wakeupChannel->setCallback(this);
    _wakeupChannel->enableReading();
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

void EventLoop::queueLoop(IRun* pRun)
{
    _pendingFunctors.push_back(pRun);
    wakeup();
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
    vector<IRun*> tempRuns;
    tempRuns.swap(_pendingFunctors);
    vector<IRun*>::iterator it;
    for(it = tempRuns.begin(); it != tempRuns.end(); ++it)
    {
        (*it)->run();
    }
}
