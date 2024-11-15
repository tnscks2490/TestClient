#include "Actor.h"
#include "TcpClient.h"
#include "MoveComp.h"
#include "ProjectileComp.h"

Actor::Actor()
{
}

Actor::~Actor()
{
    //sprite->removeFromParent();
    //delete mMoveComp;
    //delete mPJComp;

    //TcpClient::get()->SendActorMessage(this, 'd');
}

void Actor::update(float delta)
{
    if (mPJComp)  mPJComp->update(delta);
    if (mMoveComp) mMoveComp->update(delta);
}

