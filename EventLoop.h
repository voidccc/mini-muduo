//author voidccc
#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "Declear.h"
#include "IChannelCallback.h"
#include "Task.h"
#include "Mutex.h"

#include <vector>
using namespace std;

class EventLoop : public IChannelCallback
{
    public:
        EventLoop();
        ~EventLoop();
        void loop();
        void update(Channel* pChannel);
        void queueInLoop(Task& task);
        void runInLoop(Task& task);
        int runAt(Timestamp when, IRun0* pRun);
        int runAfter(double delay, IRun0* pRun);
        int runEvery(double interval, IRun0* pRun);
        void cancelTimer(int timerfd);
        bool isInLoopThread();

        virtual void handleRead();
        virtual void handleWrite();
    private:
        void wakeup();
        int createEventfd();
        void doPendingFunctors();
        bool _quit;
        bool _callingPendingFunctors;
        Epoll* _pPoller;
        int _eventfd;
        const pid_t _threadId;
        Channel* _pEventfdChannel;
        MutexLock _mutex;
        vector<Task> _pendingFunctors;
        TimerQueue* _pTimerQueue;
};

#endif
