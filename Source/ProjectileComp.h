#pragma once


class Actor;

class ProjectileComp
{
public:
    ProjectileComp(Actor* actor);
    ~ProjectileComp();


    void update(float delta);



public:


    float mTimer = 0.f;
    Actor* mActor;
};
