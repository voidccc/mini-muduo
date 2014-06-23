#include "Task.h"

#include "IRun.h"

Task::Task(IRun0* func)
    :_func0(func)
    ,_func2(NULL)
    ,_param(NULL)
{

}

Task::Task(IRun2* func, const string& str, void* param)
    :_func0(NULL)
    ,_func2(func)
    ,_str(str)
    ,_param(param)
{

}

void Task::doTask()
{
    if(_func0) {
        _func0->run0();
    } else {
        _func2->run2(_str, _param);
    }
}
