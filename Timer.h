#ifndef TIMER_H
#define TIMER_H

#include "Declear.h"

#include "IRun.h"

class Timer
{
    public:
        Timer(Timestamp stamp, IRun0* pRun, double interval)
            :_stamp(stamp)
             ,_id(stamp)
             ,_pRun0(pRun)
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
        void timeout()
        {
            _pRun0->run0();
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
        IRun0* _pRun0;
        double _interval;//seconds
};

#endif
