#pragma once
#include "Actor.h"

class MoveComp
{
public:
    MoveComp(Actor* actor);
    ~MoveComp();

    void update(float delta);
    void SetTarget(ax::Vec2 target);
    float length(ax::Vec2 v1, ax::Vec2 v2);
    ax::Vec2 Vec2DNormalized(ax::Vec2 target);
    bool IsArrive();
    void Do_Moving();

public:
    Actor* mActor;

    bool IsMoving = false;

    ax::Vec2 mTarget;
    ax::Vec2 mVelocity;

    float mSpeed = 50.0f;

    float mTimer = 0.f;
};
