/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
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

#include "HelloWorldScene.h"

#include "app.h"
#include "editor.h"
#include "explorer.h"
#include "keyinput.h"
#include "render.h"
#include "scripting.h"
#include "search.h"

keybinding_t keybinding;
render_t renderer;
scripting_t scripting;
search_t search;
app_t app;

typedef std::map<cocos2d::EventKeyboard::KeyCode,
        std::chrono::high_resolution_clock::time_point> key_press_map_t;

static key_press_map_t keyPressed;
static key_press_map_t keyReleased;

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    renderer.initialize();
    scripting.initialize();
    keybinding.initialize();

    app.configure(NULL, 0);
    app.setupColors();
    app.applyTheme();

    // editor
#if defined(__ANDROID__)
    char* file = "/sdcard/main.cpp";
#else
    char* file = "/home/iceman/Developer/textmate/tashlar/src/main.cpp";
#endif

    app.openEditor(file);
    app.explorer.setRootFromFile(file);

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    displayMap = new CursesTileMap();
    if (!displayMap->initWithTMXFile("blank.tmx")) {
        app_t::log("error loading tmx");
    } else {
        Size msz = displayMap->getMapSize();
        Size tsz = displayMap->getTileSize();
        app_t::log("map: %d %d %d %d", (int)msz.width, (int)msz.height, (int)tsz.width, (int)tsz.height);
    }
    displayMap->setTerminalSize(Size(80,25));
    displayMap->setScale(3.0);
    // displayMap->positionAndScale(CursesTileMap::MapAlign::Center, CursesTileMap::MapAlign::Middle, 0);
    this->addChild(displayMap);

    scheduleUpdate();
    // update(100);

    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event){
        Vec2 loc = event->getCurrentTarget()->getPosition();
        if(keyPressed.find(keyCode) == keyPressed.end()){
            keyPressed[keyCode] = std::chrono::high_resolution_clock::now();
        }
    };

    eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event){
        // remove the key.  std::map.erase() doesn't care if the key doesnt exist
        keyPressed.erase(keyCode);
        keyReleased[keyCode] = std::chrono::high_resolution_clock::now();
    };

    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, displayMap);


    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}

void HelloWorld::update(float dt)
{
    int delta = 100;
    renderer.update(dt);
    
    // std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();

    key_press_map_t::iterator it;
    it = keyPressed.find(EventKeyboard::KeyCode::KEY_LEFT_ARROW);
    if (it != keyPressed.end()) {
        app.input(0, "left");
    }
    it = keyPressed.find(EventKeyboard::KeyCode::KEY_RIGHT_ARROW);
    if (it != keyPressed.end()) {
        app.input(0, "right");
    }
    it = keyPressed.find(EventKeyboard::KeyCode::KEY_UP_ARROW);
    if (it != keyPressed.end()) {
        app.input(0, "up");
    }
    it = keyPressed.find(EventKeyboard::KeyCode::KEY_DOWN_ARROW);
    if (it != keyPressed.end()) {
        app.input(0, "down");
    }

    for (it = keyReleased.begin(); it != keyReleased.end(); it++)
    {
        EventKeyboard::KeyCode key = it->first;
        char ch = (int)key - 124 + 'a';
        if (ch >= 'a' && ch <= 'z') {
            app.input(ch, "");
            // app_t::log("press: %c", );
        }
    }
    keyReleased.clear();

    app.update(delta);
    app.preLayout();
    app.layout(0, 0, 80, 50);
    app.preRender();
    app.render();

    renderer.render();

    displayMap->update(dt);
}