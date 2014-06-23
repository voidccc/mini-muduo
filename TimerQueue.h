// author voidccc
#ifndef TIMERQUEUE_H
#define TIMERQUEUE_H

#include "Declear.h"
#include "IChannelCallback.h"
#include "IRun.h"
#include "Timestamp.h"

#include <vector>
#include <set>
using namespace std;

class TimerQueue : public IChannelCallback
                 , public IRun2
{
    public:

        TimerQueue(EventLoop* pLoop);
        ~TimerQueue();
        void doAddTimer(Timer* timer);
        void doCancelTimer(Timer* timer);
        long addTimer(IRun0* pRun,
                Timestamp when,
                double interval);
        void cancelTimer(long timerId);

        virtual void run2(const string& str, void* timer);

        virtual void handleRead();
        virtual void handleWrite();

    private:
        typedef std::pair<Timestamp, Timer*> Entry;
        typedef std::set<Entry> TimerList;

        int createTimerfd();
        vector<TimerQueue::Entry> getExpired(Timestamp now);
        void readTimerfd(int timerfd, Timestamp now);
        void reset(const vector<Entry>& expired, Timestamp now);
        void resetTimerfd(int timerfd, Timestamp stamp);
        bool insert(Timer* pItem);
        struct timespec howMuchTimeFromNow(Timestamp when);

        int _timerfd;
        TimerList _pTimers;
        EventLoop* _pLoop;
        Channel* _pTimerfdChannel;
};

#endif
