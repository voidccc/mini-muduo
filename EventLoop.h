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
        class Runner
        {
            public:
               Runner(IRun* r, void* p)
                   :_pRun(r)
                   ,_param(p){};
               void doRun()
               {
                   _pRun->run(_param);
               }
            private:
               IRun* _pRun;
               void* _param;
        };
        EventLoop();
        ~EventLoop();
        void loop();
        void update(Channel* pChannel);
        void queueLoop(IRun* pRun, void* param);
        int runAt(Timestamp when, IRun* pRun);
        int runAfter(double delay, IRun* pRun);
        int runEvery(double interval, IRun* pRun);
        void cancelTimer(int timerfd);

        void virtual handleRead();
        void virtual handleWrite();
    private:
        void wakeup();
        int createEventfd();
        void doPendingFunctors();
        bool _quit;
        Epoll* _pPoller;
        int _eventfd;
        Channel* _pEventfdChannel;
        vector<Runner> _pendingFunctors;
        TimerQueue* _pTimerQueue;
};

#endif
