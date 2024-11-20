/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#pragma once
#include "axmol.h"
#include "PrePacket.h"
class Actor;


class MainScene : public ax::Scene
{
    typedef std::vector<Actor*> ActorList;
    enum class GameState
    {
        init = 0,
        update,
        pause,
        end,
        menu1,
        menu2,
    };
    
public:

    ~MainScene();
    bool init() override;
    void update(float delta) override;

    // touch
    void onTouchesBegan(const std::vector<ax::Touch*>& touches, ax::Event* event);
    void onTouchesMoved(const std::vector<ax::Touch*>& touches, ax::Event* event);
    void onTouchesEnded(const std::vector<ax::Touch*>& touches, ax::Event* event);

    // mouse
    void onMouseDown(ax::Event* event);
    void onMouseUp(ax::Event* event);
    void onMouseMove(ax::Event* event);
    void onMouseScroll(ax::Event* event);

    // Keyboard
    void onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event);
    void onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event);


    // PhysicsWorld

    bool onContactBegin(ax::PhysicsContact& contact);
    bool onContactSeparate(ax::PhysicsContact& contact);

    // a selector callback
    void menuCloseCallback(ax::Object* sender);


    void SelectPlayActor(int charNum);
    Actor* CreateActor(PK_Data data);

    void Decording();

    void CloneSetVisible(bool isvisible);

    void SetTileNodes();
    void OnOffTile();

public:
    std::vector<ax::Node* > mTileNodes;

    bool TileOn = false;
    int width  = 80;
    int height = 45;

private:
    GameState _gameState = GameState::init;

    // 서버와 연동을 위한 클라이언트

    ActorList mActorList;
    ActorList mPJList;
    // 실제 플레이 캐릭터
    Actor* mPlayActor = nullptr;
    int mPlayerNum    = 0;


    ax::PhysicsWorld* mPhysicsWorld = nullptr;
    // 캐릭터 선택 견본
    // 선택후 전부 인비져블할 예정
    Node* mAngel = nullptr;
    Node* mDarkAngel = nullptr;
    Node* mFarmer    = nullptr;


 


};

