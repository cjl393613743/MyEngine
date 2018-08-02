#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <ctime>
#include "heart_beat.h"
#include "heap.h"

using namespace std;

static unsigned int times = 0;
static unsigned int short iHeartBeatType = 0;
static MinHeap *pMinHeap = NULL;

bool CallOut(unsigned int delay, unsigned int period, unsigned int times, TimerCallBackPoiner pCallBack, void *pArgs)
{
    if(pMinHeap == NULL || iHeartBeatType == 0) return false;

    Timer *pTimer = new Timer();
    pTimer->pCallBack = pCallBack;
    pTimer->pArgs = pArgs;
    pTimer->period = period;
    pTimer->timeout = time(0) + delay;
    pTimer->evalTimes = times;

    SuperValueTimer *pSuperValueTimer = new SuperValueTimer(pTimer);

    return pMinHeap->InsertHeapElem(pSuperValueTimer);
}

static void EvalTimeOutTimer()
{
    unsigned int now = time(0);
    while(1)
    {
        SuperValueTimer *pSuperValueTimer = (SuperValueTimer*)pMinHeap->GetTopHeapElem();
        if(pSuperValueTimer == NULL) break;
        Timer *pTimer = pSuperValueTimer->u.pTimer;
        unsigned int timeout = pTimer->timeout;
        if(timeout > now) break;

        pMinHeap->RemoveHeapElem(pSuperValueTimer);
        pTimer->Eval();
        if(pTimer->evalTimes > 0) pTimer->evalTimes -= 1;
        if(pTimer->evalTimes == 0)
        {
            delete pTimer;
            delete pSuperValueTimer;
        }else
        {
            pTimer->timeout = now + pTimer->period;
            pMinHeap->InsertHeapElem(pSuperValueTimer);
        }
    }
}

static void SignalCallBack(int signal)
{
    alarm(1);
    cout<<"第"<<++times<<"次回调。"<<"当前时间:"<<time(0)<<endl;

    EvalTimeOutTimer();
}

static void SignalLoop()
{
    signal(SIGALRM, SignalCallBack);
    alarm(1);

    while(1)
    {
        pause();
    }
}

bool InitHeartBeat(unsigned short int type = TYPE_HEART_BEAT_SIGNAL)
{
    if(pMinHeap != NULL || iHeartBeatType != 0) return false;

    pMinHeap = new MinHeap();
    switch(type)
    {
        case TYPE_HEART_BEAT_SIGNAL:
            iHeartBeatType = TYPE_HEART_BEAT_SIGNAL;
            break;
        default:
            return false;
    }

    return true;
}

bool StartHeartBeat()
{
    if(pMinHeap == NULL || iHeartBeatType == 0) return false;

    switch(iHeartBeatType)
    {
        case TYPE_HEART_BEAT_SIGNAL:
            SignalLoop();
            break;
        default:
            return false;
    }

    return true;
}
