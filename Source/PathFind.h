#pragma once
#include "JPSPathFinder.h"



class PathFind
{
public:

    PathFind(int width, int height);
    ~PathFind() {}

    
public:

    jpspath::CollisionMap::SharedPtr mColMap = nullptr;

};
