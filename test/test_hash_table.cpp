#include <iostream>
#include "hash_table.h"
#include "svalue.h"

using namespace std;

int main()
{
    HashTable mp = HashTable(3);

    SuperValueInt objSvalueInt = SuperValueInt(5);
    SuperValueInt *pSvalue = &objSvalueInt;
    mp[1] = pSvalue;

    SuperValueInt objSvalueInt2 = SuperValueInt(10);
    SuperValueInt *pSvalue2 = &objSvalueInt2;
    mp[5] = pSvalue2;

    mp.PrintHashTable();
    mp.RemoveHashTableElem(1);
    mp.PrintHashTable();

    return 0;
}
