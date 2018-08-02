#ifndef __SVALUE_H__
#define __SVALUE_H__

typedef void (*TimerCallBackPoiner)(void*); 

const unsigned int TYPE_SVALUE_STRING = 1;
const unsigned int TYPE_SVALUE_INT = 2;
const unsigned int TYPE_SVALUE_DOUBLE = 3;
const unsigned int TYPE_SVALUE_TIMER = 4;

class Timer
{
    public:
        TimerCallBackPoiner pCallBack;
        void *pArgs;
        unsigned int period;
        unsigned int timeout;
        unsigned int evalTimes;

        Timer();
        void Eval();
};

union U
{
    char *str;
    int number;
    double real;

    Timer *pTimer;
};

class SuperValueBase
{
    public:
        unsigned int type;
        U u;

        virtual void PrintSuperValue() = 0;
        virtual bool operator>(const SuperValueBase &base) = 0;
        //virtual bool operator>=(const SuperValueBase &base) = 0;
        virtual bool operator<(const SuperValueBase &base) = 0;
        //virtual bool operator<=(const SuperValueBase &base) = 0;
        //virtual bool operator==(const SuperValueBase &base) = 0;
        //virtual bool operator!=(const SuperValueBase &base) = 0;
};

class SuperValueInt : public SuperValueBase
{
    public:
        SuperValueInt(int i);

        virtual void PrintSuperValue();
        virtual bool operator>(const SuperValueBase &base);
        //virtual bool operator>=(const SuperValueBase &base);
        virtual bool operator<(const SuperValueBase &base);
        //virtual bool operator<=(const SuperValueBase &base);
        //virtual bool operator==(const SuperValueBase &base);
        //virtual bool operator!=(const SuperValueBase &base);
};


class SuperValueTimer : public SuperValueBase
{
    public:
        SuperValueTimer(Timer *pTimer);

        virtual void PrintSuperValue();
        virtual bool operator>(const SuperValueBase &base);
        //virtual bool operator>=(const SuperValueBase &base);
        virtual bool operator<(const SuperValueBase &base);
        //virtual bool operator<=(const SuperValueBase &base);
        //virtual bool operator==(const SuperValueBase &base);
        //virtual bool operator!=(const SuperValueBase &base);
};

#endif
