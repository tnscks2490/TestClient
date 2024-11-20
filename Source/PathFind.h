#pragma once
#include "JPSPathFinder.h"



class PathFind
{
public:

    PathFind(int width, int height);
    ~PathFind() {}

    jpspath::CollisionMap::SharedPtr mColMap = nullptr;



};
