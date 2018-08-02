#ifndef __HEAP_H__
#define __HEAP_H__

#include "config.h"
#include "svalue.h"

const unsigned short int INIT_HEAP_DEFAULT_SIZE = 1024;
const unsigned short int INIT_HEAP_MAX_SIZE = 1 << (sizeof(INIT_HEAP_MAX_SIZE)*8-1);

class HeapElem
{
    public:
        unsigned short int index;
        SuperValueBase *pData;

        HeapElem();
        HeapElem(unsigned short int index, SuperValueBase *pData);
        ~HeapElem();

        void PrintHeapElem();
};

class MinHeap
{
    private:
        unsigned short int iSize;
        unsigned short int iTotalSize;
        HeapElem **pElemPointerList;

        bool HeapGrow();
        void AdjustHeapUp(int i);
        void AdjustHeapDown(int i);

    public:
        MinHeap();
        MinHeap(SuperValueBase **pSuperValueList, unsigned short int n);
        ~MinHeap();

        void BuildHeap();
        bool InsertHeapElem(SuperValueBase *pData);
        bool RemoveHeapElem(SuperValueBase *pData);
        SuperValueBase* GetTopHeapElem();
        void PrintHeapElem();
};

#endif
