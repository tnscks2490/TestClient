#include "ProjectileComp.h"
#include "Actor.h"


ProjectileComp::ProjectileComp(Actor* actor)
{
    mActor            = actor;
    mActor->mPJComp = this;
}

ProjectileComp::~ProjectileComp() {}

void ProjectileComp::update(float delta)
{



    mTimer += delta;
    if (mTimer > 5.0f)
    {
        mTimer = 0.f;

        delete mActor;
    }
}
