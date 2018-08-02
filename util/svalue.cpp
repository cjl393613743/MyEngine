#include <iostream>
#include "svalue.h"

using namespace std;

/************************Timer********************************/
Timer::Timer(TimerCallBackPoiner pCallBack, void *pArgs) : pCallBack(pCallBack), pArgs(pArgs){}

void Timer::Eval()
{
    this->pCallBack(this->pArgs);
}

/************************SuperValueInt************************/
SuperValueInt::SuperValueInt(int i)
{
    this->type = TYPE_SVALUE_INT;
    this->u.number = i;
}

void SuperValueInt::PrintSuperValue()
{
    cout<<this->u.number;
}

bool SuperValueInt::operator>(const SuperValueBase &base)
{
    return this->u.number > base.u.number;
}

bool SuperValueInt::operator<(const SuperValueBase &base)
{
    return this->u.number < base.u.number;
}


/************************SuperValueTimer************************/
SuperValueTimer::SuperValueTimer(Timer *pTimer)
{
    this->type = TYPE_SVALUE_TIMER;
    this->u.pTimer = pTimer;
}

void SuperValueTimer::PrintSuperValue()
{
    cout<<this->u.pTimer->timeout;
}

bool SuperValueTimer::operator>(const SuperValueBase &base)
{
    return this->u.pTimer->timeout > base.u.pTimer->timeout;
}

bool SuperValueTimer::operator<(const SuperValueBase &base)
{
    return this->u.pTimer->timeout < base.u.pTimer->timeout;
}
