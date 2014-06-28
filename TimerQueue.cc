//author voidccc

#include <sys/timerfd.h>
#include <inttypes.h>
#include <stdio.h>
#include <strings.h>

#include "TimerQueue.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Timestamp.h"
#include "Timer.h"
#include "Task.h"

#include <iostream>
#include <string>

using namespace std;

#define UINTPTR_MAX 0xffffffff

TimerQueue::TimerQueue(EventLoop *pLoop)
    :_timerfd(createTimerfd())
    ,_pLoop(pLoop)
    ,_pTimerfdChannel(new Channel(_pLoop, _timerfd)) // Memory Leak !!!
{
    _pTimerfdChannel->setCallback(this);
    _pTimerfdChannel->enableReading();
}

TimerQueue::~TimerQueue()
{
    ::close(_timerfd);
}

void TimerQueue::run2(const string& str, void* timer)
{
    if(str == "addtimer")
    {
        doAddTimer((Timer*)timer);
    }
    else if(str == "canceltimer")
    {
        doCancelTimer((Timer*)timer);
    }
    else { }
}

void TimerQueue::doAddTimer(Timer* pTimer)
{
    bool earliestChanged = insert(pTimer);
    if(earliestChanged)
    {
        resetTimerfd(_timerfd, pTimer->getStamp());
    }
}

void TimerQueue::doCancelTimer(Timer* pTimer)
{
    Entry e(pTimer->getId(), pTimer);
    TimerList::iterator it;
    for(it = _pTimers.begin(); it != _pTimers.end(); ++it)
    {
        if(it->second == pTimer)
        {
            _pTimers.erase(it);
            break;
        }
    }
}

///////////////////////////////////////
/// Add a timer to the system
/// @param pRun: callback interface
/// @param when: time
/// @param interval:
///     0 = happen only once, no repeat
///     n = happen after the first time every n seconds
/// @return the process unique id of the timer
long TimerQueue::addTimer(IRun0* pRun, Timestamp when, double interval)
{
    Timer* pAddTimer = new Timer(when, pRun, interval); //Memory Leak !!!
    string str("addTimer");
    Task task(this, str, pAddTimer);
    _pLoop->queueInLoop(task);
    return (long)(pAddTimer);
}

void TimerQueue::cancelTimer(long timerId)
{
    Timer* pCancel = (Timer*)(timerId);
    string str("canceltimer");
    Task task(this, str, pCancel);
    _pLoop->queueInLoop(task);
}

void TimerQueue::handleRead()
{
    Timestamp now(Timestamp::now());
    readTimerfd(_timerfd, now);

    vector<Entry> expired = getExpired(now);
    vector<Entry>::iterator it;
    for(it = expired.begin(); it != expired.end(); ++it)
    {
        it->second->timeout();
    }
    reset(expired, now);
}

void TimerQueue::handleWrite()
{}

int TimerQueue::createTimerfd()
{
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
            TFD_NONBLOCK | TFD_CLOEXEC);
    if(timerfd < 0)
    {
        cout << "failed in timerfd_create" << endl;
    }
    return timerfd;
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{
    std::vector<Entry> expired;
    Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
    TimerList::iterator end = _pTimers.lower_bound(sentry);
    copy(_pTimers.begin(), end, back_inserter(expired));
    _pTimers.erase(_pTimers.begin(), end);
    return expired;
}

void TimerQueue::readTimerfd(int timerfd, Timestamp now)
{
    uint64_t howmany;
    ssize_t n = ::read(timerfd, &howmany, sizeof(howmany));
    if (n != sizeof(howmany))
    {
        cout << "Timer::readTimerfd() error " << endl;
    }
}

void TimerQueue::reset(const vector<Entry>& expired, Timestamp now)
{
    vector<Entry>::const_iterator it;
    for(it = expired.begin(); it != expired.end(); ++it)
    {
        if(it->second->isRepeat())
        {
            it->second->moveToNext();
            insert(it->second);
        }
    }

    Timestamp nextExpire;
    if(!_pTimers.empty())
    {
        nextExpire = _pTimers.begin()->second->getStamp();
    }
    if(nextExpire.valid())
    {
        resetTimerfd(_timerfd, nextExpire);
    }
}

void TimerQueue::resetTimerfd(int timerfd, Timestamp stamp)
{
    struct itimerspec newValue;
    struct itimerspec oldValue;
    bzero(&newValue, sizeof(newValue));
    bzero(&oldValue, sizeof(oldValue));
    newValue.it_value = howMuchTimeFromNow(stamp);
    int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
    if(ret)
    {
        cout << "timerfd_settime error" << endl;
    }
}

bool TimerQueue::insert(Timer* pTimer)
{
    bool earliestChanged = false;
    Timestamp when = pTimer->getStamp();
    TimerList::iterator it = _pTimers.begin();
    if(it == _pTimers.end() || when < it->first)
    {
        earliestChanged = true;
    }
    pair<TimerList::iterator, bool> result
       = _pTimers.insert(Entry(when, pTimer));
    if(!(result.second))
    {
        cout << "_pTimers.insert() error " << endl;
    }

    return earliestChanged;
}

struct timespec TimerQueue::howMuchTimeFromNow(Timestamp when)
{
    int64_t microseconds = when.microSecondsSinceEpoch()
        - Timestamp::now().microSecondsSinceEpoch();
    if (microseconds < 100)
    {
        microseconds = 100;
    }
    struct timespec ts;
    ts.tv_sec = static_cast<time_t>(
            microseconds / Timestamp::kMicroSecondsPerSecond);
    ts.tv_nsec = static_cast<long>(
            (microseconds % Timestamp::kMicroSecondsPerSecond) * 1000);
    return ts;
}
