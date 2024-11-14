#include "MoveComp.h"
#include "TcpClient.h"
MoveComp::MoveComp(Actor* actor)
{
    mActor = actor;
    mActor->mMoveComp = this;
    IsMoving          = false;
    
}

MoveComp::~MoveComp() {}

void MoveComp::update(float delta)
{
    mVelocity = ax::Vec2(0, 0);


    mTimer += delta;

    if (mTimer > 1.0f)
    {
        mTimer = 0;
        printf("ㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁ\n");
        printf("엑터 위치 :: x : %f :: y : %f\n",
            mActor->sprite->getPosition().x,mActor->sprite->getPosition().y);
        printf("ㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁ\n");
    }

    if (IsMoving)
    {
        Do_Moving();
        mVelocity.normalize();
        ax::Vec2 pos = mActor->sprite->getPosition();
        pos += mVelocity * delta * mSpeed;
        mActor->sprite->setPosition(pos);
    }
    
}

void MoveComp::SetTarget(ax::Vec2 target)
{
    mTarget  = target;
    IsMoving = true;
}



// 현재위치와 목표위치를 비교 후 도착 여부 판별
bool MoveComp::IsArrive()
{
    ax::Vec2 mypos = mActor->sprite->getPosition();
    ax::Vec2 target = mTarget;

    float m = length(mActor->sprite->getPosition(), mTarget);
    //printf("목적지와 현재위치와의 거리 : %f\n");
    if (3.0 > m)
        return true;
    else
        return false;
}

// 두 지점사이의 거리
float MoveComp::length(ax::Vec2 v1, ax::Vec2 v2)
{
    float dx  = v2.x - v1.x;
    float dy = v2.y - v1.y;

    return sqrt(dx * dx + dy * dy);
}

// 벡터 정규화
ax::Vec2 MoveComp::Vec2DNormalized(ax::Vec2 target)
{
    ax::Vec2 vec = target;

    double vector_length = vec.length();

    if (vector_length > 2)
    {
        vec.x /= vector_length;
        vec.y /= vector_length;
    }

    return vec;
}

void MoveComp::Do_Moving()
{
    if (IsArrive())
    {
        IsMoving = false;
    }
    else
        mVelocity += mSpeed * Vec2DNormalized(mTarget - mActor->sprite->getPosition());
}
