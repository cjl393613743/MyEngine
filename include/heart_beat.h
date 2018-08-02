#ifndef __HEART_BEAT_H__
#define __HEART_BEAT_H__

bool SignalCallOut(void (*pCallBack)(void*), void* pArgs);
void SignalLoop();
bool InitHeartBeat();

#endif
