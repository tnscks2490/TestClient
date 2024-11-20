#include "ProjectileComp.h"
#include "MoveComp.h"
#include "Actor.h"


ProjectileComp::ProjectileComp(Actor* actor)
{
    mActor            = actor;
    mActor->mPJComp = this;
}

ProjectileComp::~ProjectileComp() {}

void ProjectileComp::update(float delta)
{

    if (mTimer == 0)
    {
        if (mActor->mMoveComp)
        {
            mActor->mMoveComp->mSpeed = 300.f;
        }
    }

    mTimer += delta;
    if (mTimer > 5.0f)
    {
        mTimer = 0.f;
        mActor->mRoot->setVisible(false);     
    }
}
