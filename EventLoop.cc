//author voidccc

#include "EventLoop.h"
#include "Channel.h"
#include "Epoll.h"

EventLoop::EventLoop()
    :_quit(false)
    ,_poller(new Epoll()) // Memory Leak !!!
{
}

EventLoop::~EventLoop()
{}

void EventLoop::loop()
{
    while(!_quit)
    {
        vector<Channel*> channels;
        _poller->poll(&channels);

        vector<Channel*>::iterator it;
        for(it = channels.begin(); it != channels.end(); ++it)
        {
            (*it)->handleEvent(); 
        }
    }
}

void EventLoop::update(Channel* channel)
{
    _poller->update(channel);
}
