#include <iostream>
#include "svalue.h"
#include "hash_table.h"

using namespace std;

/************************Timer********************************/
Timer::Timer() : pCallBack(NULL), pArgs(NULL), iPeriod(0), iTimeOut(0), iEvalTimes(1) {}

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
    cout<<this->u.pTimer->iTimeOut;
}

bool SuperValueTimer::operator>(const SuperValueBase &base)
{
    return this->u.pTimer->iTimeOut > base.u.pTimer->iTimeOut;
}

bool SuperValueTimer::operator<(const SuperValueBase &base)
{
    return this->u.pTimer->iTimeOut < base.u.pTimer->iTimeOut;
}


/************************SuperValueHashTable*********************/
SuperValueHashTable::SuperValueHashTable(HashTable *pHashTable)
{
    this->type = TYPE_SVALUE_HASH_TABLE;
    this->u.pHashTable = pHashTable;
}

void SuperValueHashTable::PrintSuperValue()
{
    this->u.pHashTable->PrintHashTable();
}

bool SuperValueHashTable::operator>(const SuperValueBase &base)
{
    return this->u.pHashTable->GetSize() > base.u.pHashTable->GetSize();
}

bool SuperValueHashTable::operator<(const SuperValueBase &base)
{
    return this->u.pHashTable->GetSize() < base.u.pHashTable->GetSize();
}
