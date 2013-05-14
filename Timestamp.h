//author voidccc
#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <sys/types.h>
#include <string>
using namespace std;

class Timestamp
{
public:
    Timestamp(double microSeconds = 0.0);
    ~Timestamp();
    bool valid();
    int64_t microSecondsSinceEpoch();
    string toString() const;

    static Timestamp now();
    static Timestamp nowAfter(double seconds);
    static double nowMicroSeconds();
    static const int kMicroSecondsPerSecond = 1000 * 1000;
private:
    int64_t _microSecondsSinceEpoch;
};
bool operator <(Timestamp l, Timestamp r);
bool operator ==(Timestamp l, Timestamp r);

#endif
