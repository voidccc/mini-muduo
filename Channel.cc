//author voidccc
#include "Channel.h"
#include "IChannelCallBack.h"
#include <sys/epoll.h>
#include <iostream>

Channel::Channel(int epollfd, int sockfd)
    :_epollfd(epollfd)
    ,_sockfd(sockfd)
    ,_events(0)
    ,_revents(0)
    ,_callBack(NULL)
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
    struct epoll_event ev;
    ev.data.ptr = this;
    ev.events = _events;
    epoll_ctl(_epollfd, EPOLL_CTL_ADD, _sockfd, &ev);
}
