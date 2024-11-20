#pragma once
#include "axmol.h"


class TileNode
{
public:
    TileNode() {}
    ~TileNode() {}


    static TileNode* CreateTileNode(ax::Node* parent);

    void SetPosition(ax::Vec2 pos);


public:
    ax::Node* mRoot = nullptr;
    ax::DrawNode* drawNode = nullptr;
    bool IsPass = true;
};

