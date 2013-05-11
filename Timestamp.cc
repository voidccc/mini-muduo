//author voidccc

#include <sys/time.h>
#include <stdio.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#undef __STDC_FORMAT_MACROS

#include "Timestamp.h"

#include <iostream>

Timestamp::Timestamp(double microSeconds)
    :_microSecondsSinceEpoch(microSeconds)
{}

Timestamp::~Timestamp()
{}

bool Timestamp::valid()
{
    return _microSecondsSinceEpoch > 0;
}

int64_t Timestamp::microSecondsSinceEpoch()
{
    return _microSecondsSinceEpoch;
}

string Timestamp::toString() const
{
  char buf[32] = {0};
  int64_t seconds = _microSecondsSinceEpoch / kMicroSecondsPerSecond;
  int64_t microseconds = _microSecondsSinceEpoch % kMicroSecondsPerSecond;
  snprintf(buf, sizeof(buf)-1, "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
  return buf;
}

Timestamp Timestamp::now()
{
    return Timestamp(Timestamp::nowMicroSeconds());
}

Timestamp Timestamp::nowAfter(double seconds)
{
    return Timestamp(Timestamp::nowMicroSeconds() + kMicroSecondsPerSecond * seconds);
}

double Timestamp::nowMicroSeconds()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t seconds = tv.tv_sec;
    return seconds * kMicroSecondsPerSecond + tv.tv_usec;
}

bool operator<(Timestamp l, Timestamp r)
{
    return l.microSecondsSinceEpoch() < r.microSecondsSinceEpoch();
}

bool operator==(Timestamp l, Timestamp r)
{
    cout << "operator ==" << endl;
    return l.microSecondsSinceEpoch() == r.microSecondsSinceEpoch();
}

