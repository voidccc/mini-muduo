//author voidccc
#ifndef IRUN_H
#define IRUN_H

#include <string>

using namespace std;

class IRun0
{
    public:
        virtual void run0() = 0;
};

class IRun2
{
    public:
        virtual void run2(const string& str, void* param) = 0;
};

#endif
