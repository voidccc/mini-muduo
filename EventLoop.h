//author voidccc
#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "Declear.h"
#include "IChannelCallback.h"
#include "IRun.h"

#include <vector>
using namespace std;

class EventLoop : public IChannelCallback
{
    public:
        EventLoop();
        ~EventLoop();
        void loop();
        void update(Channel* pChannel);
        void queueLoop(IRun* pRun);

        void virtual handleRead();
        void virtual handleWrite();
    private:
        void wakeup();
        int createEventfd();
        void doPendingFunctors();
        bool _quit;
        Epoll* _pPoller;
        int _eventfd;
        Channel* _wakeupChannel;
        vector<IRun*> _pendingFunctors;
};

#endif
