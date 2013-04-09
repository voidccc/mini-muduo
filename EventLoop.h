//author voidccc
#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "Declear.h"

class EventLoop
{
    public:
        EventLoop();
        ~EventLoop();
        void loop();
        void update(Channel* pChannel);
    private:
        bool _quit;
        Epoll* _pPoller;
};

#endif
