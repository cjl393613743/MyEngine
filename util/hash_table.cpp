#include <iostream>
#include "hash_table.h"
#include "svalue.h"

using namespace std;

/************************HashElem*********************/
HashElem::HashElem(int iKey, SuperValueBase *base)
{
    this->iKeyType = TYPE_HASH_ELEM_KEY_INT;
    this->uKey.iKey = iKey;
    this->pBase = base;
    this->pNext = NULL;
}

/************************HashTable*********************/
HashTable::HashTable(unsigned int iSize = 7)
{
    iSize |= iSize >> 1;
    iSize |= iSize >> 2;
    iSize |= iSize >> 4;

    this->iSize = ++iSize;
    this->pElemPointerList = new HashElem*[iSize];
    for(unsigned int i = 0; i < this->iSize; ++i) this->pElemPointerList[i] = NULL;
}

HashTable::~HashTable()
{
    delete[] this->pElemPointerList;
}

inline int HashTable::HashTableHash(int key)
{
    return (this->iSize-1) & key;
}

bool HashTable::HashTableGrow()
{
    if(this->iSize == INIT_HASH_TABLE_MAX_SIZE) return false;

    unsigned int iOldSize = this->iSize;

    this->iSize = (this->iSize << 1) + 1;
    HashElem **pOldElemPointerList = this->pElemPointerList;
    this->pElemPointerList = new HashElem*[this->iSize];
    for(unsigned int i = 0; i < this->iSize; ++i) this->pElemPointerList[i] = NULL;

    for(unsigned int i = 0; i < iOldSize; ++i)
    {
        HashElem *pHashElem = pOldElemPointerList[i];
        while(pHashElem != NULL)
        {
            int hash = -1;
            switch(pHashElem->iKeyType)
            {
                case TYPE_HASH_ELEM_KEY_INT:
                    hash = this->HashTableHash(pHashElem->uKey.iKey);
                    break;
                case TYPE_HASH_ELEM_KEY_STR:
                    break;
            }

            HashElem *pLastHashElem = this->pElemPointerList[hash];
            HashElem *pTmpHashElem = this->pElemPointerList[hash];
            while(pTmpHashElem != NULL)
            {
                pLastHashElem = pTmpHashElem;
                pTmpHashElem = pTmpHashElem->pNext;
            }

            if(pLastHashElem == NULL)
            {
                this->pElemPointerList[hash] = pHashElem;
            }else
            {
                pLastHashElem->pNext = pHashElem;
            }

            HashElem *pTmpLastHashElem = pHashElem->pNext;
            pHashElem->pNext = NULL;
            pHashElem = pTmpLastHashElem;
        }
    }

    delete[] pOldElemPointerList;
}

SuperValueBase* HashTable::RemoveHashTableElem(int iKey)
{
    int hash = this->HashTableHash(iKey);
    HashElem *pLastHashElem = this->pElemPointerList[hash];
    HashElem *pTmpHashElem = this->pElemPointerList[hash];
    while(pTmpHashElem != NULL)
    {
        if(pTmpHashElem->uKey.iKey == iKey)
        {
            SuperValueBase *pBase = pTmpHashElem->pBase;

            if(pLastHashElem == pTmpHashElem)
            {
                this->pElemPointerList[hash] = pTmpHashElem->pNext;
            }else
            {
                pLastHashElem->pNext = pTmpHashElem->pNext;
            }

            this->iCurSize -= 1;

            delete pTmpHashElem;

            return pBase;
        }

        pLastHashElem = pTmpHashElem;
        pTmpHashElem = pTmpHashElem->pNext;
    }

    return NULL;
}

HashElem* HashTable::InsertHashTableElem(int iKey, SuperValueBase *base)
{
    if(this->iCurSize == this->iSize && this->HashTableGrow()) return NULL;

    this->iCurSize += 1;
    int hash = this->HashTableHash(iKey);
    HashElem *pLastHashElem = this->pElemPointerList[hash];
    HashElem *pTmpHashElem = this->pElemPointerList[hash];
    while(pTmpHashElem != NULL)
    {
        if(pTmpHashElem->uKey.iKey == iKey)
        {
            pTmpHashElem->pBase = base;
            return pTmpHashElem;
        }

        pLastHashElem = pTmpHashElem;
        pTmpHashElem = pTmpHashElem->pNext;
    }

    HashElem *pHashElem = new HashElem(iKey, base);

    if(pLastHashElem == NULL)
    {
        this->pElemPointerList[hash] = pHashElem;
    }else
    {
        pLastHashElem->pNext = pHashElem;
    }

    return pHashElem;
}

SuperValueBase*& HashTable::operator[](int iKey)
{
    int hash = this->HashTableHash(iKey);

    HashElem *pHashElem = this->pElemPointerList[hash];
    while(pHashElem != NULL)
    {
        if(pHashElem->iKeyType == TYPE_HASH_ELEM_KEY_INT && pHashElem->uKey.iKey == iKey) break;
        pHashElem = pHashElem->pNext;
    }

    if(pHashElem == NULL) pHashElem = this->InsertHashTableElem(iKey, NULL);

    return pHashElem->pBase;
}

void HashTable::PrintHashTable()
{
    cout<<"{\n";
    for(unsigned int i = 0; i < this->iSize; ++i)
    {
        HashElem *pHashElem = this->pElemPointerList[i];
        while(pHashElem != NULL)
        {
            cout<<"    "<<pHashElem->uKey.iKey<<" : ";
            SuperValueBase *pBase = pHashElem->pBase;
            pBase->PrintSuperValue();
            pHashElem = pHashElem->pNext;
            cout<<",\n";
        }
    }
    cout<<"}";
}

unsigned int HashTable::GetSize()
{
    return this->iCurSize;
}
