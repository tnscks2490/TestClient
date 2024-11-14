#include "Actor.h"
#include "MoveComp.h"
#include "TcpClient.h"

Actor::~Actor()
{
    TcpClient::get()->SendActorMessage(this, 'd');
}

void Actor::update(float delta)
{
    if (mMoveComp) mMoveComp->update(delta);
}
