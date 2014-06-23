//author voidccc
#ifndef TASK_H
#define TASK_H 

#include "Declear.h"
#include <string>

class Task
{
    public:
        Task(IRun0* func);
        Task(IRun2* func, const std::string& str, void* param);
        void doTask();
    private:
        IRun0* _func0;
        IRun2* _func2;
        std::string _str;
        void* _param;
};

#endif
