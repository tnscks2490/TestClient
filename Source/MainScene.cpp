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

#include "MainScene.h"
#include "TcpClient.h"
#include "Actor.h"
#include "MoveComp.h"
#include "PrePacket.h"

using namespace ax;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf(
        "Depending on how you compiled you might have to add 'Content/' in front of filenames in "
        "MainScene.cpp\n");
}

MainScene::~MainScene()
{
    mPlayActor->~Actor();
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = _director->getVisibleSize();
    auto origin = _director->getVisibleOrigin();
    auto safeArea = _director->getSafeAreaRect();
    auto safeOrigin = safeArea.origin;

    TcpClient::get();

    

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
        AX_CALLBACK_1(MainScene::menuCloseCallback, this));

    if (closeItem == nullptr || closeItem->getContentSize().width <= 0 || closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = safeOrigin.x + safeArea.size.width - closeItem->getContentSize().width / 2;
        float y = safeOrigin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // Some templates (uncomment what you  need)
    auto touchListener = EventListenerTouchAllAtOnce::create();
    touchListener->onTouchesBegan = AX_CALLBACK_2(MainScene::onTouchesBegan, this);
    touchListener->onTouchesMoved = AX_CALLBACK_2(MainScene::onTouchesMoved, this);
    touchListener->onTouchesEnded = AX_CALLBACK_2(MainScene::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    auto mouseListener           = EventListenerMouse::create();
    mouseListener->onMouseDown   = AX_CALLBACK_1(MainScene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    auto keyboardListener           = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed  = AX_CALLBACK_2(MainScene::onKeyPressed, this);
    keyboardListener->onKeyReleased = AX_CALLBACK_2(MainScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithFixedPriority(keyboardListener, 11);



    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(
            Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }
    // add "HelloWorld" splash screen"

    mAngel = Sprite::create("Angel.png"sv);
    mAngel->setPosition(300, 100);
    this->addChild(mAngel, 0);

    mDarkAngel = Sprite::create("Dark_Angel.png"sv);
    mDarkAngel->setPosition(600, 100);
    this->addChild(mDarkAngel, 0);

    mFarmer = Sprite::create("Farmer.png"sv);
    mFarmer->setPosition(900, 100);
    this->addChild(mFarmer, 0);


    auto drawNode = DrawNode::create();
    drawNode->setPosition(Vec2(0, 0));
    addChild(drawNode);

    drawNode->drawRect(safeArea.origin + Vec2(1, 1), safeArea.origin + safeArea.size, Color4B::BLUE);


    // scheduleUpdate() is required to ensure update(float) is called on every loop
    scheduleUpdate();

    return true;
}


void MainScene::onTouchesBegan(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
        AXLOG("onTouchesBegan detected, X:%f  Y:%f", t->getLocation().x, t->getLocation().y);
    }
}

void MainScene::onTouchesMoved(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
        AXLOG("onTouchesMoved detected, X:%f  Y:%f", t->getLocation().x, t->getLocation().y);
    }
}

void MainScene::onTouchesEnded(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
        AXLOG("onTouchesEnded detected, X:%f  Y:%f", t->getLocation().x, t->getLocation().y);
    }
}

void MainScene::onMouseDown(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);

    ax::Vec2 mousePos = ax::Vec2(e->getCursorX(), e->getCursorY());
    printf("x : %f :: y : %f\n", mousePos.x, mousePos.y);
    
    if (mPlayActor)
    {
        mPlayActor->mMoveComp->SetTarget(mousePos);
        TcpClient::get()->SendActorMessage(mPlayActor, 't');
    }
    
}

void MainScene::onMouseUp(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    AXLOG("onMouseUp detected, Key: %d", static_cast<int>(e->getMouseButton()));
}

void MainScene::onMouseMove(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    AXLOG("onMouseMove detected, X:%f  Y:%f", e->getCursorX(), e->getCursorY());
}

void MainScene::onMouseScroll(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    AXLOG("onMouseScroll detected, X:%f  Y:%f", e->getScrollX(), e->getScrollY());
}

void MainScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event)
{
    if (mPlayActor == nullptr)
    {
        switch (code)
        {
        case ax::EventKeyboard::KeyCode::KEY_1:
            SelectPlayActor(1);
            AXLOG("현재 플레이 하고있는 캐릭터는 천사입니다.");
            break;
        case ax::EventKeyboard::KeyCode::KEY_2:
            SelectPlayActor(2);
            AXLOG("현재 플레이 하고있는 캐릭터는 악마입니다.");
            break;
        case ax::EventKeyboard::KeyCode::KEY_3: 
            SelectPlayActor(3);
            AXLOG("현재 플레이 하고있는 캐릭터는 인간입니다.");
            break;
        default:
            AXLOG("범위를 벗어납니다. 다시 선택해주세요");
            break;
        }
    }
    else
    {

        Vec2 pos = mPlayActor->sprite->getPosition();
        switch (code)
        {
        case ax::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            pos.x = pos.x - 5;
            break;
        case ax::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            pos.x = pos.x + 5;
            break;
        case ax::EventKeyboard::KeyCode::KEY_UP_ARROW:
            pos.y = pos.y + 5;
            break;
        case ax::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            pos.y = pos.y - 5;
            break;
        default:
            break;
        }

        mPlayActor->sprite->setPosition(pos);
        TcpClient::get()->SendActorMessage(mPlayActor, 'm');
    }
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event)
{
    AXLOG("onKeyReleased, keycode: %d", static_cast<int>(code));
}

void MainScene::update(float delta)
{
    switch (_gameState)
    {
    case GameState::init:
    {
        _gameState = GameState::update;
        break;
    }

    case GameState::update:
    {
        // 데이터를 받아오기
        timeval timeout = {0, 0};
        if (TcpClient::get() && TcpClient::get()->Select(timeout))
        {
            if (mPlayActor)
            {
                if (TcpClient::get()->RecvData())
                {
                    Decording();
                }
            }
        }

        // 받아온 데이터를 토대로 함수 실행
        for (auto actor : mActorList)
        {
            if (actor && actor->mMoveComp)
                actor->update(delta);
        }

    }

    case GameState::pause:
    {
        /////////////////////////////
        // Add your codes below...like....
        //
        // anyPauseStuff()

        break;
    }

    case GameState::menu1:
    {  /////////////////////////////
        // Add your codes below...like....
        //
        // UpdateMenu1();
        break;
    }

    case GameState::menu2:
    {  /////////////////////////////
        // Add your codes below...like....
        //
        // UpdateMenu2();
        break;
    }

    case GameState::end:
    {  /////////////////////////////
        // Add your codes below...like....
        //
        // CleanUpMyCrap();
        menuCloseCallback(this);
        break;
    }

    };  // switch
}


void MainScene::menuCloseCallback(ax::Object* sender)
{
    TcpClient::get()->SendActorMessage(nullptr, 'd');
    // Close the axmol game scene and quit the application
    _director->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use
     * _director->end() as given above,instead trigger a custom event created in RootViewController.mm
     * as below*/

     // EventCustom customEndEvent("game_scene_close_event");
     //_eventDispatcher->dispatchEvent(&customEndEvent);
}

void MainScene::SelectPlayActor(int charNum)
{
    Actor* actor = CreateActor(charNum, TcpClient::get()->GetID());

    mPlayActor = actor;
    mActorList.push_back(actor);
    this->addChild(actor->sprite, 0);



    TcpClient::get()->SendActorMessage(actor, 'c');

    // 캐릭터 선택창 비활성화
    mAngel->setVisible(false);
    mDarkAngel->setVisible(false);
    mFarmer->setVisible(false);
}

Actor* MainScene::CreateActor(int charNum, int id)
{
    std::string_view name;
    switch (charNum)
    {
    case 1: name = "Angel.png"sv;       break;
    case 2: name = "Dark_Angel.png"sv;  break;
    case 3: name = "Farmer.png"sv;      break;
    default:
        break;
    }
    Actor* actor = new Actor();
    actor->sprite = Sprite::create(name);
    actor->mCharNum = charNum;
    actor->sprite->setPosition(500, 400);
    actor->mID = id;
    auto move  = new MoveComp(actor);
    return actor;


}

void MainScene::Decording()
{
    char buf[1024] = {0};
    bool check = true;
    int Idx        = 0;
    short Len      = 0;

    while (check)
    {

        PK_Head haed;
        PK_Data data;


        memcpy(&haed, TcpClient::get()->mRecvBuf + Idx, sizeof(PK_Head));
        Idx += sizeof(PK_Head);

        if (haed.dataLen == 0)
        {
            for (int i = 0; i < Idx-3; i++)
            {
                TcpClient::get()->mRecvBuf[i] = 0;
            }
            check = false;
            continue;
        }

        memcpy(&data, TcpClient::get()->mRecvBuf + Idx, sizeof(PK_Data));
        Idx += sizeof(PK_Data);

       /* if (data.clientID == TcpClient::get()->GetID())
            continue;*/


        switch (data.action)
        {
        case 'c':
        {
            bool check = false;
            for (auto actor : mActorList)
            {
                if (actor && actor->mID == data.clientID)
                {
                    check = true;
                    break;
                }
            }
            if (!check)
            {
                Actor* actor = CreateActor(data.charNum, data.clientID);
                mActorList.push_back(actor);
                this->addChild(actor->sprite, 0);
                TcpClient::get()->SendActorMessage(mPlayActor, 'c');
            }
        }
        break;
        case 'm':
        {
            for (auto actor : mActorList)
            {
                if (actor && actor->mID == data.clientID)
                    actor->sprite->setPosition(data.pos);
            }
        }
        break;
        case 't':
        {
            for (auto actor : mActorList)
            {
                if (actor && actor->mID == data.clientID)
                {
                    actor->mMoveComp->SetTarget(data.pos);
                }
            }
        }
        break;
        case 'd':
        {
            for (auto actor : mActorList)
            {
                if (actor && actor->mID == data.clientID)
                {
                    actor->sprite->removeFromParent();
                    actor = nullptr;
                }
            }
        }
        break;
        }
        
    } 
}




