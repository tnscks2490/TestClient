#pragma once
#include "Actor.h"
#include "JPSPathFinder.h"


class MoveComp
{
public:
    MoveComp(Actor* actor);
    ~MoveComp();

    void update(float delta);
    void SetTarget(ax::Vec2 target);
    void SetPath(std::list<jpspath::Coord> Path);
    float length(ax::Vec2 v1, ax::Vec2 v2);
    ax::Vec2 Vec2DNormalized(ax::Vec2 target);
    bool IsArrive();
    void Do_Moving();
    void CheckTargetList();

public:
    Actor* mActor;

    bool IsMoving = false;
    bool IsPathMoving = false;

    std::list<ax::Vec2> mTargetList;


    ax::Vec2 mTarget;
    ax::Vec2 mVelocity;
    float mSpeed = 100.0f;
    float mTimer = 0.f;
};
