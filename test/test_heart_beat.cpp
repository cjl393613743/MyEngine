#include <iostream>
#include "heart_beat.h"

using namespace std;

void PrintMsg(void *pArgs)
{
    char *str = (char*)pArgs;
    cout<<str<<endl;
}

int main()
{
    InitHeartBeat(TYPE_HEART_BEAT_SIGNAL);

    char str1[] = "1111111111111111";
    char str2[] = "2222222222222222";
    char str3[] = "3333333333333333";

    CallOut(1, 5, 1, PrintMsg, str1);
    CallOut(2, 5, 5, PrintMsg, str2);
    CallOut(3, 5, -1, PrintMsg, str3);

    bool flag = StartHeartBeat();
    cout<<"心跳启动结果:"<<flag<<endl;

    return 0;
}
