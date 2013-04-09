//author voidccc

#include "EventLoop.h"
#include "Channel.h"
#include "Epoll.h"

EventLoop::EventLoop()
    :_quit(false)
    ,_pPoller(new Epoll()) // Memory Leak !!!
{
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
    }
}

void EventLoop::update(Channel* pChannel)
{
    _pPoller->update(pChannel);
}
