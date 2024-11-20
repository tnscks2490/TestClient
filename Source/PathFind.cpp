#include "PathFind.h"

#include <iostream>
#include <chrono>
#include <time.h>
#include <stdarg.h>



PathFind::PathFind(int width, int height)
{
    mColMap = std::make_shared<jpspath::CollisionMap>();

    int32_t GridWidth = width;
    int32_t GridHeight = height;

    mColMap->Create(GridWidth, GridHeight);
}
