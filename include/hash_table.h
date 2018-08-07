#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

const unsigned short int TYPE_HASH_ELEM_KEY_INT = 1;
const unsigned short int TYPE_HASH_ELEM_KEY_STR = 2;

const unsigned int INIT_HASH_TABLE_MAX_SIZE = 0xFFFFFFF + 1;

class SuperValueBase;

union HashElemKey
{
    int iKey;
    char *sKey;
};

class HashElem
{
    public:
        unsigned short int iKeyType;
        HashElemKey uKey;
        SuperValueBase *pBase;
        HashElem *pNext;

        HashElem(int iKey, SuperValueBase *base);
};

class HashTable
{
    public:
        unsigned int iSize;
        unsigned int iCurSize;
        HashElem **pElemPointerList;
        
        inline int HashTableHash(int key);
        bool HashTableGrow();

    public:
        HashTable(unsigned int iSize);
        ~HashTable();
        SuperValueBase* RemoveHashTableElem(int iKey);
        HashElem* InsertHashTableElem(int iKey, SuperValueBase *base);
        SuperValueBase*& operator[](int iKey);
        void PrintHashTable();
        unsigned int GetSize();
};

#endif
