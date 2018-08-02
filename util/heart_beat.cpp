#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <ctime>
#include "heart_beat.h"
#include "heap.h"

using namespace std;

static unsigned int times = 0;
static MinHeap *pMinHeap = NULL;

bool SignalCallOut(TimerCallBackPoiner pCallBack, void *pArgs)
{
    if(pMinHeap == NULL) return false;

    pCallBack();

    return true;
}

static void SignalCallBack(int signal)
{
    alarm(1);
    cout<<"第"<<++times<<"次回调。"<<"当前时间:"<<time(0)<<endl;
}

void SignalLoop()
{
    signal(SIGALRM, SignalCallBack);
    alarm(1);

    while(1)
    {
        pause();
    }
}

bool InitHeartBeat()
{
    if(pMinHeap != NULL) return false;

    pMinHeap = new MinHeap();

    return true;
}
