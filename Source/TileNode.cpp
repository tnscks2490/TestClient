#include "TileNode.h"

TileNode* TileNode::CreateTileNode(ax::Node* parent)
{
    TileNode* mTileNode = new TileNode();
    mTileNode->mRoot      = ax::Node::create();
    parent->addChild(mTileNode->mRoot);


    mTileNode->drawNode = ax::DrawNode::create();
    mTileNode->drawNode->drawRect(ax::Vec2(-8, -8), ax::Vec2(8, 8), ax::Color4F::RED);
    mTileNode->mRoot->addChild(mTileNode->drawNode);

    return mTileNode;
}

void TileNode::SetPosition(ax::Vec2 pos)
{
    if (mRoot)
    {
        mRoot->setPosition(pos);
    }
}
