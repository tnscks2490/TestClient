#pragma once
#include "axmol.h"

class MoveComp;
class ProjectileComp;


class Actor
{
public:
    Actor();
    ~Actor();

    void update(float delta);
    void AddChild(ax::Node* node);
    ax::Node* mRoot    = nullptr;
    ax::Node* sprite = nullptr;
    short mID          = -1;
    char charNum     = 0;

public:
    //컴포넌트 모임
    MoveComp* mMoveComp = nullptr;
    ProjectileComp* mPJComp = nullptr;
};

