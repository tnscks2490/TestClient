#pragma once
#include "axmol.h"


class TileNode
{
public:
    TileNode() {}
    ~TileNode() {}


    static TileNode* CreateTileNode(ax::Node* parent);

    void SetPosition(ax::Vec2 pos);
    void ChangeDrawNode();

public:
    ax::Node* mRoot = nullptr;
    ax::DrawNode* drawRect = nullptr;
    ax::DrawNode* drawSolidRect = nullptr;
    bool IsPass = true;
    int idx                = -1;
};

