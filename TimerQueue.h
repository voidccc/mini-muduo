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
{
    public:
        class Timer
        {
            public:
                Timer(Timestamp stamp, IRun* pRun, double interval)
                    :_stamp(stamp)
                    ,_id(stamp)
                    ,_pRun(pRun)
                    ,_interval(interval)
                {}
                Timestamp getStamp()
                {
                    return _stamp;
                }
                Timestamp getId()
                {
                    return _id;
                }
                void run()
                {
                    _pRun->run(this);
                }

                bool isRepeat()
                {
                    return _interval > 0.0;
                }

                void moveToNext()
                {
                    _stamp = Timestamp::nowAfter(_interval);
                }
            private:
               Timestamp _stamp;
               Timestamp _id;
               IRun* _pRun;
               double _interval;//seconds
        };

        class AddTimerWrapper : public IRun
        {
            public:
                AddTimerWrapper(TimerQueue* pQueue)
                    :_pQueue(pQueue){};
                virtual void run(void* param)
                {
                    _pQueue->doAddTimer(param);
                };
            private:
                TimerQueue* _pQueue;
        };

        class CancelTimerWrapper : public IRun
        {
            public:
                CancelTimerWrapper(TimerQueue* pQueue)
                    :_pQueue(pQueue){};
                virtual void run(void* param)
                {
                    _pQueue->doCancelTimer(param);
                }
            private:
                TimerQueue* _pQueue;
        };

        TimerQueue(EventLoop* pLoop);
        ~TimerQueue();
        void doAddTimer(void* param);
        void doCancelTimer(void* param);
        long addTimer(IRun* pRun,
                Timestamp when,
                double interval);
        void cancelTimer(long timerId);

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
        AddTimerWrapper* _pAddTimerWrapper;
        CancelTimerWrapper* _pCancelTimerWrapper;
};

#endif
