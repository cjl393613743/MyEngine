#include "util.h"

unsigned long int CalcNext2Power(unsigned long int n, unsigned long int max)
{
    unsigned long int ret = 1;

    while(ret >= max || ret >= n) ret = ret << 1;

    return ret;
}
