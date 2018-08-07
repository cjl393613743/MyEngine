#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <ctime>
#include "heart_beat.h"
#include "hash_table.h"
#include "heap.h"

using namespace std;

static unsigned int times = 0;

static unsigned int gIndex = 0;
static unsigned int short gHeartBeatType = 0;
static MinHeap *gMinHeap = NULL;
static HashTable *gHashTable = NULL;

inline static bool CheckInitHeartBeat()
{
    if(gMinHeap == NULL || gHashTable == NULL || gHeartBeatType == 0) return 0;
    return 1;
}

inline static unsigned int GenHeartBeatIndex()
{
    if(gIndex == 0XFFFF) gIndex = 0;
    ++gIndex;
}

unsigned int CallOut(unsigned int iDelay, unsigned int iPeriod, unsigned int iTimes, TimerCallBackPoiner pCallBack, void *pArgs)
{
    if(!CheckInitHeartBeat()) return 0;

    unsigned int iIndex = GenHeartBeatIndex();

    Timer *pTimer = new Timer();
    pTimer->pCallBack = pCallBack;
    pTimer->pArgs = pArgs;
    pTimer->iPeriod = iPeriod;
    pTimer->iTimeOut = time(0) + iDelay;
    pTimer->iEvalTimes = iTimes;
    pTimer->iHashTableIndex = iIndex;

    SuperValueTimer *pSuperValueTimer = new SuperValueTimer(pTimer);

    if(NULL == gHashTable->InsertHashTableElem(iIndex, pSuperValueTimer))
    {
        delete pSuperValueTimer;
        return 0;
    }

    bool insertFlag = gMinHeap->InsertHeapElem(pSuperValueTimer);
    if(!insertFlag)
    {
        gHashTable->RemoveHashTableElem(iIndex);
        delete pSuperValueTimer;
        return 0;
    }

    return iIndex;
}

bool RemoveCallOut(unsigned int iIndex)
{
    SuperValueTimer *pSuperValueTimer = (SuperValueTimer*)gHashTable->operator[](iIndex);
    if(pSuperValueTimer == NULL) return false;

    Timer *pTimer = pSuperValueTimer->u.pTimer;
    gMinHeap->RemoveHeapElem(pSuperValueTimer);
    gHashTable->RemoveHashTableElem(iIndex);
    delete pTimer;
    delete pSuperValueTimer;
}

static void EvalTimeOutTimer()
{
    unsigned int now = time(0);
    while(1)
    {
        SuperValueTimer *pSuperValueTimer = (SuperValueTimer*)gMinHeap->GetTopHeapElem();
        if(pSuperValueTimer == NULL) break;
        Timer *pTimer = pSuperValueTimer->u.pTimer;
        unsigned int iTimeOut = pTimer->iTimeOut;
        if(iTimeOut > now) break;

        gMinHeap->RemoveHeapElem(pSuperValueTimer);
        pTimer->Eval();
        if(pTimer->iEvalTimes > 0) pTimer->iEvalTimes -= 1;
        if(pTimer->iEvalTimes == 0)
        {
            unsigned int iHashTableIndex = pTimer->iHashTableIndex;
            gHashTable->RemoveHashTableElem(iHashTableIndex);

            delete pTimer;
            delete pSuperValueTimer;
        }else
        {
            pTimer->iTimeOut = now + pTimer->iPeriod;
            gMinHeap->InsertHeapElem(pSuperValueTimer);
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
    if(CheckInitHeartBeat()) return false;

    gMinHeap = new MinHeap();
    gHashTable = new HashTable(INIT_HAERT_BEAT_HASH_TABLE_SIZE);
    switch(type)
    {
        case TYPE_HEART_BEAT_SIGNAL:
            gHeartBeatType = TYPE_HEART_BEAT_SIGNAL;
            break;
        default:
            return false;
    }

    return true;
}

bool StartHeartBeat()
{
    if(!CheckInitHeartBeat()) return false;

    switch(gHeartBeatType)
    {
        case TYPE_HEART_BEAT_SIGNAL:
            SignalLoop();
            break;
        default:
            return false;
    }

    return true;
}
