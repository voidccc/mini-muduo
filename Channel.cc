//author voidccc

#include <sys/epoll.h>

#include "Channel.h"
#include "IChannelCallback.h"
#include "EventLoop.h"

#include <iostream>
using namespace std;

Channel::Channel(EventLoop* pLoop, int sockfd)
    :_sockfd(sockfd)
    ,_events(0)
    ,_revents(0)
    ,_pCallback(NULL)
    ,_pLoop(pLoop)
{
}

void Channel::setCallback(IChannelCallback* pCallback)
{
    _pCallback = pCallback;
}

void Channel::setRevents(int revents)
{
    _revents = revents;
}

void Channel::handleEvent()
{
   if(_revents & EPOLLIN)
   {
      _pCallback->onIn(_sockfd);
   }
}

void Channel::enableReading()
{
    _events |= EPOLLIN;
    update();
}

void Channel::update()
{
    _pLoop->update(this);
}

int Channel::getEvents()
{
    return _events;
}

int Channel::getSockfd()
{
    return _sockfd;
}
