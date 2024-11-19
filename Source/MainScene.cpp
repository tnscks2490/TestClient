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
#include "ProjectileComp.h"
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
    if (!Scene::initPhysicsWorld())
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

    // 피직스월드 작성하기


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

    auto contactListener               = EventListenerPhysicsContact::create();
    contactListener->onContactBegin    = AX_CALLBACK_1(MainScene::onContactBegin, this);
    contactListener->onContactSeparate = AX_CALLBACK_1(MainScene::onContactSeparate, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);


    mPhysicsWorld = getPhysicsWorld();
    //mPhysicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL );
    getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);


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
    this->addChild(mAngel);

    mDarkAngel = Sprite::create("Dark_Angel.png"sv);
    mDarkAngel->setPosition(600, 100);
    this->addChild(mDarkAngel);

    mFarmer = Sprite::create("Farmer.png"sv);
    mFarmer->setPosition(900, 100);
    this->addChild(mFarmer);


   /* auto drawNode = DrawNode::create();
    drawNode->setPosition(Vec2(0, 0));
    addChild(drawNode);

    drawNode->drawRect(safeArea.origin + Vec2(1, 1), safeArea.origin + safeArea.size, Color4B::BLUE);*/


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


    if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)
    {
        if (mPlayActor)
        {
            PK_Data data;
            data.ClientID = TcpClient::get()->GetID();
            data.input    = 'r';
            data.pos      = mousePos;
            TcpClient::get()->SendActorMessage(data);
        }
    }
    else if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
    {
        if (mPlayActor)
        {
            PK_Data data;
            data.ClientID = TcpClient::get()->GetID();
            data.input    = 'l';
            data.pos      = mousePos;
            TcpClient::get()->SendActorMessage(data);
        }
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
    if (code == ax::EventKeyboard::KeyCode::KEY_P)
        getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);


    if (mPlayActor == nullptr)
    {
        PK_Data data;
        data.ClientID = TcpClient::get()->GetID();
        data.pos      = Vec2(500, 500);


        switch (code)
        {
        case ax::EventKeyboard::KeyCode::KEY_1:
            data.input = 77;  
            TcpClient::get()->SendActorMessage(data);
            break;
        case ax::EventKeyboard::KeyCode::KEY_2:
            data.input = 78;
            TcpClient::get()->SendActorMessage(data);
            break;
        case ax::EventKeyboard::KeyCode::KEY_3:
            data.input = 79;
            TcpClient::get()->SendActorMessage(data);
            break;
        default:
            AXLOG("범위를 벗어납니다. 다시 선택해주세요");
            break;
        }
    }
    
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event)
{
    if (code == ax::EventKeyboard::KeyCode::KEY_P)
        getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
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
            if (TcpClient::get()->RecvData())
            {
                Decording();
            }      
        }

        // 받아온 데이터를 토대로 함수 실행
        for (auto actor : mActorList)
        {
            if (actor)
                actor->update(delta);
        }

        for (auto actor : mPJList)
        {
            if (actor)
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


bool MainScene::onContactBegin(ax::PhysicsContact& contact)
{
    printf("충돌났음\n");
    return true;
}

bool MainScene::onContactSeparate(ax::PhysicsContact& contact)
{
    printf("충돌해제\n");
    return true;
}

void MainScene::menuCloseCallback(ax::Object* sender)
{
    //TcpClient::get()->SendActorMessage(nullptr, 'd');
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
    //Actor* actor = CreateActor(charNum, TcpClient::get()->GetID());

    /*mPlayActor = actor;
    mActorList.push_back(actor);
    this->addChild(actor->sprite, 0);*/



    //TcpClient::get()->SendActorMessage(actor, 'c');

    // 캐릭터 선택창 비활성화
    mAngel->setVisible(false);
    mDarkAngel->setVisible(false);
    mFarmer->setVisible(false);
}

Actor* MainScene::CreateActor(PK_Data data)
{
    std::string_view name;
    switch (data.input)
    {
        case 77:    name = "Angel.png"sv;       break;
        case 78:    name = "Dark_Angel.png"sv;  break;
        case 79:    name = "Farmer.png"sv;      break;
        case 108:   name = "Projectile.png"sv;  break;
        default: break;
    }

    Actor* actor    = new Actor();
    this->addChild(actor->mRoot);
    actor->mRoot->setPosition(data.pos);

    actor->sprite = Sprite::create(name);
    actor->AddChild(actor->sprite);


    actor->mID     = data.ClientID;
    actor->charNum = data.input;

    //ax::Vec2 anchor(1,1);
    //anchor = actor->sprite->getAnchorPoint();
    //actor->sprite->setAnchorPoint(anchor);

   auto bodyNode = ax::Node::create();

    ax::Vec2 bodysize(32,32);

    auto body = ax::PhysicsBody::createBox(bodysize);
    body->setContactTestBitmask(0xFFFFFFFF);
    body->setDynamic(true);
    bodyNode->setPhysicsBody(body);

    actor->AddChild(bodyNode);

    getPhysicsWorld()->setDebugDrawMask(ax::PhysicsWorld::DEBUGDRAW_ALL);



    auto drawNode = ax::DrawNode::create();
    drawNode->drawRect(Vec2(-16, -16), Vec2(16, 16), ax::Color4F::RED);
    actor->AddChild(drawNode);



    auto move  = new MoveComp(actor);

    if (data.input == 108)
    {
        Actor* mArc;
        for (auto archer : mActorList)
        {
            if (archer && archer->mID == data.ClientID)
                mArc = archer;
        }
        actor->mRoot->setPosition(mArc->mRoot->getPosition());
        auto proj = new ProjectileComp(actor);
        proj->mArcher = mArc;
        mPJList.push_back(actor);
        
    }
    else
    {
        mActorList.push_back(actor);
    }
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

        switch (data.input)
        {
        case 77:
        case 78:
        case 79:
        {
            
            if (mPlayActor == nullptr && data.ClientID == TcpClient::get()->GetID())
            {
                Actor* actor = CreateActor(data);
                mPlayActor = actor;
               
            }
            if (mPlayActor)
            {
                bool check = false;
                for (auto actor : mActorList)
                {
                    if (actor && actor->mID == data.ClientID)
                    {
                        check = true;
                    }
                }
                if (!check)
                {
                    Actor* actor = CreateActor(data);
                    PK_Data d;
                    d.ClientID = TcpClient::get()->GetID();
                    d.input    = mPlayActor->charNum;
                    d.pos      = mPlayActor->sprite->getPosition();
                    TcpClient::get()->SendActorMessage(d);
                }

                
            }
        }
        break;
        case 108:
        {
            for (auto actor : mActorList)
            {
                if (actor && actor->mID == data.ClientID)
                {
                    Actor* pro = CreateActor(data);
                    pro->mMoveComp->SetTarget(data.pos);
                }
            }
        }
        break;
        case 114: 
            for (auto actor : mActorList)
            {
                if (actor && actor->mID == data.ClientID)
                    actor->mMoveComp->SetTarget(data.pos);
            }
        break;
        ///////////////////////
        default: break;
        }
    } 
}




