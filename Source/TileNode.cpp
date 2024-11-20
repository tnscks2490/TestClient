#include "TileNode.h"

TileNode* TileNode::CreateTileNode(ax::Node* parent)
{
    TileNode* mTileNode = new TileNode();
    mTileNode->mRoot      = ax::Node::create();
    parent->addChild(mTileNode->mRoot);


    mTileNode->drawRect = ax::DrawNode::create();
    mTileNode->drawRect->drawRect(ax::Vec2(-8, -8), ax::Vec2(8, 8), ax::Color4F::RED);
    mTileNode->mRoot->addChild(mTileNode->drawRect);
    mTileNode->drawRect->setVisible(mTileNode->IsPass);

    mTileNode->drawSolidRect = ax::DrawNode::create();
    mTileNode->drawSolidRect->drawSolidRect(ax::Vec2(-8, -8), ax::Vec2(8, 8), ax::Color4F::RED);
    mTileNode->mRoot->addChild(mTileNode->drawSolidRect);
    mTileNode->drawSolidRect->setVisible(!mTileNode->IsPass);

    return mTileNode;
}

void TileNode::ChangeDrawNode()
{
    IsPass = !IsPass;

    drawRect->setVisible(IsPass);
    drawSolidRect->setVisible(!IsPass);
}

void TileNode::SetPosition(ax::Vec2 pos)
{
    if (mRoot) mRoot->setPosition(pos);
}


