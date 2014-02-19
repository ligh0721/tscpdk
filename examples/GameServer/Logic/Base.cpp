/* 
 * File:   Base.cpp
 * Author: thunderliu
 * 
 * Created on 2013��12��9��, ����12:42
 */

#include "CommInc.h"
#include "Base.h"


// CKeyGen 

int CKeyGen::nextKey()
{
    static int iKey = 0;
    if (!iKey)
    {
        srand(time(NULL));
        iKey = 1000 + rand() % 9000;
    }
    
    return ++iKey;
}
