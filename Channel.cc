//author voidccc

#include <sys/epoll.h>

#include "Channel.h"
#include "IChannelCallBack.h"
#include "EventLoop.h"

#include <iostream>

Channel::Channel(EventLoop* loop, int sockfd)
    :_sockfd(sockfd)
    ,_events(0)
    ,_revents(0)
    ,_callBack(NULL)
    ,_loop(loop)
{
}

void Channel::setCallBack(IChannelCallBack* callBack)
{
    _callBack = callBack;
}

void Channel::setRevents(int revents)
{
    _revents = revents;
}

void Channel::handleEvent()
{
   if(_revents & EPOLLIN)
   {
      _callBack->OnIn(_sockfd);
   }
}

void Channel::enableReading()
{
    _events |= EPOLLIN;
    update();
}

void Channel::update()
{
    _loop->update(this);
}

int Channel::getEvents()
{
    return _events;
}

int Channel::getSockfd()
{
    return _sockfd;
}
