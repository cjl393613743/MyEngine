#include <iostream>
#include "heap.h"

using namespace std;

const unsigned int n = 10;
const unsigned int total = 20;

int main()
{
    SuperValueInt **pSuperValueIntPointerList = new SuperValueInt*[total];
    for(int i = 0; i < n; ++i)
    {
        SuperValueInt *pTmp = new SuperValueInt(total-i);
        pSuperValueIntPointerList[i] = pTmp;
    }

    //创建堆
    MinHeap objMinHeap = MinHeap((SuperValueBase**)pSuperValueIntPointerList, n);
    objMinHeap.PrintHeapElem();

    //插入新元素
    for(int i = n; i < total; ++i)
    {
        SuperValueInt *pTmp = new SuperValueInt(total-i);
        pSuperValueIntPointerList[i] = pTmp;
        objMinHeap.InsertHeapElem(pTmp);
    }
    objMinHeap.PrintHeapElem();

    //删除新元素
    for(int i = 0; i < total; i += 2)
    {
        objMinHeap.RemoveHeapElem(pSuperValueIntPointerList[i]);
    }
    objMinHeap.PrintHeapElem();

    //析构数据
    for(int i = 0; i < total; ++i)
    {
        delete pSuperValueIntPointerList[i];
    }

    delete[] pSuperValueIntPointerList;
    
    cout<<"=======================end============="<<endl;

    return 0;
}
