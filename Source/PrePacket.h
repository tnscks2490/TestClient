#pragma once
#include "axmol.h"



#pragma pack(push,1)


struct PK_Data
{
    char action  = 0;
    short clientID = -1;
    short charNum  = -1;
    ax::Vec2 pos;
};


struct PK_Head
{
    char UniqueID          = 12;
    unsigned short dataLen = sizeof(PK_Data);
};



#pragma pack(pop)
