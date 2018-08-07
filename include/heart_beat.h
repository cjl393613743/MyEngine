#ifndef __HEART_BEAT_H__
#define __HEART_BEAT_H__

const unsigned short int INIT_HAERT_BEAT_HASH_TABLE_SIZE = 65535;

const unsigned short int TYPE_HEART_BEAT_SIGNAL = 1;

unsigned int CallOut(unsigned int delay, unsigned int period, unsigned int times, void (*pCallBack)(void*), void* pArgs);
bool RemoveCallOut(unsigned int index);
bool InitHeartBeat(unsigned short int type);
bool StartHeartBeat();

#endif
