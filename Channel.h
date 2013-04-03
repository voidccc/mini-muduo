//author voidccc
#ifndef CHANNEL_H
#define CHANNEL_H

#include "Declear.h"

class Channel
{
    public:
        Channel(EventLoop* pLoop, int sockfd);
        ~Channel();
        void setCallback(IChannelCallback* pCallback);
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
        IChannelCallback* _pCallback;
        EventLoop* _pLoop;
};

#endif
