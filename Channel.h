//author voidccc
#ifndef CHANNEL_H
#define CHANNEL_H

#include "Declear.h"

class Channel
{
    public:
        Channel(EventLoop* loop, int sockfd);
        ~Channel();
        void setCallBack(IChannelCallBack* callBack);
        void handleEvent();
        void setRevents(int revent);
        void enableReading();
        int getEvents();
        int getSockfd();
    private:
        void update();
        int _sockfd;
        int _events;
        int _revents;
        IChannelCallBack* _callBack;
        EventLoop* _loop;
};

#endif
