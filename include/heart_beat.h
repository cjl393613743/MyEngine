#ifndef __HEART_BEAT_H__
#define __HEART_BEAT_H__

const unsigned short int TYPE_HEART_BEAT_SIGNAL = 1;

bool CallOut(unsigned int delay, unsigned int period, unsigned int times, void (*pCallBack)(void*), void* pArgs);
bool InitHeartBeat(unsigned short int type);
bool StartHeartBeat();

#endif
