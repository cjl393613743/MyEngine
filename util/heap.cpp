//#include "config.h"
#include "heap.h"
#include "svalue.h"
#include "util.h"
#include <iostream>

using namespace std;

/************************HeapElem*********************/
HeapElem::HeapElem() : index(-1), pData(NULL){}

HeapElem::HeapElem(unsigned short int index, SuperValueBase *pData) : index(index), pData(pData){}

HeapElem::~HeapElem()
{
    //cout<<"析构堆元素"<<endl;
}

void HeapElem::PrintHeapElem()
{
    SuperValueBase *pBase = this->pData;
    pBase->PrintSuperValue();
}

/***********************MinHeap***********************/
MinHeap::MinHeap() : iSize(0), iTotalSize(INIT_HEAP_DEFAULT_SIZE)
{
    this->pElemPointerList = new HeapElem*[this->iTotalSize];
}

MinHeap::MinHeap(SuperValueBase **pSuperValueList, unsigned short int n)
{
    if(n > INIT_HEAP_MAX_SIZE)
    {
        //TODO:写一个日志类
        cout<<"ERROR: create a heap that was too large."<<endl;
    }

    if(INIT_HEAP_DEFAULT_SIZE >= n)
    {
        this->iTotalSize = INIT_HEAP_DEFAULT_SIZE;
    }else
    {
        this->iTotalSize = CalcNext2Power(n, INIT_HEAP_MAX_SIZE);
    }

    this->iSize = n;
    this->pElemPointerList = new HeapElem*[this->iTotalSize];

    for(int i = 0; i < n; ++i)
    {
        HeapElem *pHeapElem = new HeapElem(i, *(pSuperValueList + i));
        this->pElemPointerList[i]  = pHeapElem;
    }

    BuildHeap();
}

MinHeap::~MinHeap()
{
    //cout<<"析构小根堆"<<endl;
    
    for(int i = 0; i < this->iSize; ++i)
    {
        delete this->pElemPointerList[i];
    }

    delete[] this->pElemPointerList;
}

//堆增长
bool MinHeap::HeapGrow()
{
    if(this->iTotalSize >= INIT_HEAP_MAX_SIZE) return false;
    this->iTotalSize = iTotalSize << 1;

    HeapElem **pNewPointerList = new HeapElem*[this->iTotalSize];
    for(int i = 0; i < this->iSize; ++i)
    {
        *(pNewPointerList + i) = *(this->pElemPointerList + i);
    }

    delete[] pElemPointerList;

    pElemPointerList = pNewPointerList;

    return true;
}

//向上调整堆
void MinHeap::AdjustHeapUp(int i)
{
    HeapElem *pChild = this->pElemPointerList[i];
    for(int j = (i-1)/ 2; i > 0; j = (i-1) / 2)
    {
        HeapElem *pDad = this->pElemPointerList[j];
        if(pDad->pData->operator>(*(pChild->pData)))
        {
            this->pElemPointerList[i] = pDad;
            pDad->index = i;

            this->pElemPointerList[j] = pChild;
            pChild->index = j;

            i = j;
        }else
        {
            break;
        }
    }
}

//向下调整堆
void MinHeap::AdjustHeapDown(int i)
{
    HeapElem *pDad = this->pElemPointerList[i];
    for(int j = 2*i+1; j < this->iSize; j = 2*j+1)
    {
        if(j+1 < this->iSize && this->pElemPointerList[j]->pData->operator>(*(this->pElemPointerList[j+1]->pData))) ++j;

        HeapElem *pChild = this->pElemPointerList[j];
        if(pChild->pData->operator<(*(pDad->pData)))
        {
            this->pElemPointerList[i] = pChild;
            pChild->index = i;

            this->pElemPointerList[j] = pDad;
            pDad->index = j;

            i = j;
        }else
        {
            break;
        }
    }
}

//构建堆
void MinHeap::BuildHeap()
{
    for(int i = this->iSize/2-1; i >= 0; --i)
    {
        this->AdjustHeapDown(i);
    }
}

//插入元素
bool MinHeap::InsertHeapElem(SuperValueBase *pData)
{
    if(this->iSize >= this->iTotalSize)
    {
        bool growFlag = this->HeapGrow();
        if(growFlag == false) return false;
    }

    HeapElem *pHeapElem = new HeapElem(this->iSize, pData);
    this->pElemPointerList[this->iSize] = pHeapElem;


    this->AdjustHeapUp(this->iSize++);

    return true;
}

//删除元素
bool MinHeap::RemoveHeapElem(SuperValueBase *pData)
{
    //TODO:内建hash表
    int index = -1;
    for(int i = 0; i < this->iSize; ++i)
    {
        if(this->pElemPointerList[i]->pData == pData)
        {
            index = i;
            break;
        }
    }
    if(index == -1) return false;

    --this->iSize;
    HeapElem *pLastHeapElem = pElemPointerList[this->iSize];
    HeapElem *pRemoveHeapElem = pElemPointerList[index];
    pElemPointerList[index] = pLastHeapElem;
    pElemPointerList[this->iSize] = NULL;

    delete pRemoveHeapElem;

    if(index == this->iSize) return true;

    HeapElem *pDad = this->pElemPointerList[(index-1)/2];
    if(index != 0 && pLastHeapElem->pData->operator<(*(pDad->pData)))
    {
        this->AdjustHeapUp(index);
    }else
    {
        this->AdjustHeapDown(index);
    }

    return true;
}

//获取堆顶元素
SuperValueBase* MinHeap::GetTopHeapElem()
{
    if(this->iSize == 0) return NULL;
    return this->pElemPointerList[0]->pData;
}

//打印所有堆元素
void MinHeap::PrintHeapElem()
{
    HeapElem **tmpP = this->pElemPointerList;
    for(int i = 0; i < this->iSize; ++i)
    {
        cout<<"(";
        (*tmpP)->PrintHeapElem();
        cout<<"),";
        ++tmpP;
    }
    cout<<endl;
}
