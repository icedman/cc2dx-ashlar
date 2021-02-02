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
#include "SimpleAudioEngine.h"

#include "CCIMGUI.h"
#include "CCImGuiLayer.h"

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
std::string text;

static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

static void imguiDemo(cocos2d::Size visibleSize)
{
    CCIMGUI::getInstance()->addCallback([=](){
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | 
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoCollapse;

        ImGui::SetNextWindowSize(ImVec2(visibleSize.width, visibleSize.height), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("Hello, world!", NULL, window_flags);

        ImGui::Checkbox("Demo Window", &show_demo_window);

        app.update(100);
        app.preLayout();
        app.layout(0, 0, 80, 50);
        app.preRender();

        if (show_demo_window) {
            app.render();
            renderer.render();
        } else {
            ImGui::Text("hello");
        }

        ImGui::End();

    }, "demoid");
}

static void imguiDemo2(cocos2d::Size visibleSize)
{
    CCIMGUI::getInstance()->addCallback([=](){
    
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()!
        // You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
        
        // 2. Show a simple window that we create ourselves.
        // We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
    }, "demoid");
}

static void loadTTF() {
    ImGuiIO &io = ImGui::GetIO();
#if defined(__ANDROID__)
    io.Fonts->AddFontFromFileTTF("/sdcard/.ashlar/fonts/monospace.ttf", 12.0f, 0, io.Fonts->GetGlyphRangesDefault());
    // io.Fonts->AddFontFromFileTTF("/sdcard/.ashlar/fonts/FiraCode-Regular.ttf", 12.0f, 0, io.Fonts->GetGlyphRangesDefault());
#else
    // io.Fonts->AddFontFromFileTTF("/home/iceman/.ashlar/fonts/monospace.ttf", 22.0f, 0, io.Fonts->GetGlyphRangesDefault());
    io.Fonts->AddFontFromFileTTF("/home/iceman/.ashlar/fonts/FiraCode-Regular.ttf", 22.0f, 0, io.Fonts->GetGlyphRangesDefault());
#endif
}

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

    renderer.initialize();
    scripting.initialize();
    keybinding.initialize();

    app.configure(NULL, 0);
    app.setupColors();
    app.applyTheme();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // editor
#if defined(__ANDROID__)
    char* file = "/sdcard/main.cpp";
#else
    char* file = "/home/iceman/Developer/tashlar/src/main.cpp";
#endif

    app.openEditor(file);
    app.explorer.setRootFromFile(file);
    app.update(100);

    imguiDemo(visibleSize);
    loadTTF();
    addChild(ImGuiLayer::create());

    // auto closeItem = MenuItemImage::create(
    //                                        "CloseNormal.png",
    //                                        "CloseSelected.png",
    //                                        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    // if (closeItem == nullptr ||
    //     closeItem->getContentSize().width <= 0 ||
    //     closeItem->getContentSize().height <= 0)
    // {
    //     problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    // }
    // else
    // {
    //     float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
    //     float y = origin.y + closeItem->getContentSize().height/2;
    //     closeItem->setPosition(Vec2(x,y));
    //     addChild(closeItem);
    // }

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

    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    // app.input(0, "down");
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
            if (ch == 'a') ch = '/';
            if (ch == 'b') ch = '*';
            app.input(ch, "");
            text += ch;
        }

        // app_t::log("press: %c", ch);
    }
    keyReleased.clear();

    app.update(delta);
}