#pragma once
#include "axmol.h"

class MoveComp;

class Actor
{
public:
    Actor() {}
    ~Actor();

    void update(float delta);

    ax::Node* sprite = nullptr;
    short mCharNum  = -1;
    short mID          = -1;

public:
    //컴포넌트 모임
    MoveComp* mMoveComp = nullptr;

};

