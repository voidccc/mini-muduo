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
        void setIndex(int index);
        void enableReading();
        void enableWriting();
        void disableWriting();
        bool isWriting();
        int getEvents();
        int getfd();
        int getIndex();
    private:
        void update();
        int _sockfd;
        int _events;
        int _revents;
        int _index;
        IChannelCallback* _pCallback;
        EventLoop* _pLoop;
};

#endif
