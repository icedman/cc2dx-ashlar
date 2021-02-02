#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

cmake_minimum_required(VERSION 3.6)

set(APP_NAME hello_World)

project(${APP_NAME})

set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

include(CocosBuildSet)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)

# record sources, headers, resources...
set(GAME_SOURCE)
set(GAME_HEADER)


list(APPEND GAME_SOURCE
  ./libs/jsoncpp/jsoncpp.cpp)

list(APPEND GAME_SOURCE
    ./libs/Onigmo/regint.h
    ./libs/Onigmo/regparse.h
    ./libs/Onigmo/regenc.h
    ./libs/Onigmo/st.h
    ./libs/Onigmo/regerror.c
    ./libs/Onigmo/regparse.c
    ./libs/Onigmo/regext.c
    ./libs/Onigmo/regcomp.c
    ./libs/Onigmo/regexec.c
    ./libs/Onigmo/reggnu.c
    ./libs/Onigmo/regenc.c
    ./libs/Onigmo/regsyntax.c
    ./libs/Onigmo/regtrav.c
    ./libs/Onigmo/regversion.c
    ./libs/Onigmo/st.c
    ./libs/Onigmo/regposix.c
    ./libs/Onigmo/regposerr.c
    ./libs/Onigmo/enc/unicode.c
    ./libs/Onigmo/enc/ascii.c
    ./libs/Onigmo/enc/utf_8.c
    ./libs/Onigmo/enc/utf_16be.c
    ./libs/Onigmo/enc/utf_16le.c
    ./libs/Onigmo/enc/utf_32be.c
    ./libs/Onigmo/enc/utf_32le.c
    ./libs/Onigmo/enc/unicode/casefold.h
    ./libs/Onigmo/enc/unicode/name2ctype.h
    ./libs/Onigmo/enc/euc_jp.c
    ./libs/Onigmo/enc/shift_jis.c
    ./libs/Onigmo/enc/shift_jis.h
    ./libs/Onigmo/enc/windows_31j.c
    ./libs/Onigmo/enc/jis/props.h
    ./libs/Onigmo/enc/jis/props.kwd
    ./libs/Onigmo/enc/iso_8859.h
    ./libs/Onigmo/enc/iso_8859_1.c
    ./libs/Onigmo/enc/iso_8859_2.c
    ./libs/Onigmo/enc/iso_8859_3.c
    ./libs/Onigmo/enc/iso_8859_4.c
    ./libs/Onigmo/enc/iso_8859_5.c
    ./libs/Onigmo/enc/iso_8859_6.c
    ./libs/Onigmo/enc/iso_8859_7.c
    ./libs/Onigmo/enc/iso_8859_8.c
    ./libs/Onigmo/enc/iso_8859_9.c
    ./libs/Onigmo/enc/iso_8859_10.c
    ./libs/Onigmo/enc/iso_8859_11.c
    ./libs/Onigmo/enc/iso_8859_13.c
    ./libs/Onigmo/enc/iso_8859_14.c
    ./libs/Onigmo/enc/iso_8859_15.c
    ./libs/Onigmo/enc/iso_8859_16.c
    ./libs/Onigmo/enc/euc_tw.c
    ./libs/Onigmo/enc/euc_kr.c
    ./libs/Onigmo/enc/big5.c
    ./libs/Onigmo/enc/gb18030.c
    ./libs/Onigmo/enc/koi8_r.c
    ./libs/Onigmo/enc/koi8_u.c
    ./libs/Onigmo/enc/windows_1250.c
    ./libs/Onigmo/enc/windows_1251.c
    ./libs/Onigmo/enc/windows_1252.c
    ./libs/Onigmo/enc/windows_1253.c
    ./libs/Onigmo/enc/windows_1254.c
    ./libs/Onigmo/enc/windows_1257.c)

list(APPEND GAME_SOURCE
    ./libs/tm-parser/textmate/parser/grammar.cpp
    ./libs/tm-parser/textmate/parser/reader.cpp
    ./libs/tm-parser/textmate/parser/pattern.cpp
    ./libs/tm-parser/textmate/parser/parser.cpp
    ./libs/tm-parser/textmate/scopes/scope.cpp
    ./libs/tm-parser/textmate/scopes/types.cpp
    ./libs/tm-parser/textmate/scopes/parse.cpp
    ./libs/tm-parser/textmate/scopes/match.cpp
    ./libs/tm-parser/textmate/theme/theme.cpp
    ./libs/tm-parser/textmate/theme/util.cpp)

list(APPEND GAME_SOURCE
     ./libs/imgui/CCIMGUI.cpp
     ./libs/imgui/CCImGuiLayer.cpp
     ./libs/imgui/imgui.cpp
     ./libs/imgui/imgui_demo.cpp
     ./libs/imgui/imgui_draw.cpp
     ./libs/imgui/imgui_impl_glfw.cpp
     ./libs/imgui/imgui_impl_opengl3.cpp
     #./libs/imgui/imgui_lua.cpp
     ./libs/imgui/imgui_widgets.cpp)

list(APPEND GAME_SOURCE
    ./libs/ashlar/app.cpp
    ./libs/ashlar/block.cpp
    ./libs/ashlar/completer.cpp
    ./libs/ashlar/cursor.cpp
    ./libs/ashlar/document.cpp
    ./libs/ashlar/dots.cpp
    ./libs/ashlar/editor.cpp
    ./libs/ashlar/explorer.cpp
    ./libs/ashlar/extension.cpp
    ./libs/ashlar/gem.cpp
    ./libs/ashlar/gutter.cpp
    ./libs/ashlar/highlighter.cpp
    ./libs/ashlar/keybinding.cpp
    ./libs/ashlar/minimap.cpp
    ./libs/ashlar/popup.cpp
    ./libs/ashlar/scripting.cpp
    ./libs/ashlar/scrollbar.cpp
    ./libs/ashlar/search.cpp
    ./libs/ashlar/snapshot.cpp
    ./libs/ashlar/statusbar.cpp
    ./libs/ashlar/tabbar.cpp
    ./libs/ashlar/util.cpp
    ./libs/ashlar/view.cpp
    ./libs/ashlar/platform/keyinput_cc2dx.cpp
    ./libs/ashlar/platform/render_cc2dx.cpp)

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
    )
if(APPLE OR WINDOWS)
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif()

# add cross-platforms source files and header files 
list(APPEND GAME_SOURCE
     Classes/AppDelegate.cpp
     Classes/HelloWorldScene.cpp
     )
list(APPEND GAME_HEADER
     Classes/AppDelegate.h
     Classes/HelloWorldScene.h
     )

if(ANDROID)
    # change APP_NAME to the share library name for Android, it's value depend on AndroidManifest.xml
    set(APP_NAME MyGame)
    list(APPEND GAME_SOURCE
         proj.android/app/jni/hellocpp/main.cpp
         )
elseif(LINUX)
    list(APPEND GAME_SOURCE
         proj.linux/main.cpp
         )
elseif(WINDOWS)
    list(APPEND GAME_HEADER
         proj.win32/main.h
         proj.win32/resource.h
         )
    list(APPEND GAME_SOURCE
         proj.win32/main.cpp
         proj.win32/game.rc
         ${common_res_files}
         )
elseif(APPLE)
    if(IOS)
        list(APPEND GAME_HEADER
             proj.ios_mac/ios/AppController.h
             proj.ios_mac/ios/RootViewController.h
             )
        set(APP_UI_RES
            proj.ios_mac/ios/LaunchScreen.storyboard
            proj.ios_mac/ios/LaunchScreenBackground.png
            proj.ios_mac/ios/Images.xcassets
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/ios/main.m
             proj.ios_mac/ios/AppController.mm
             proj.ios_mac/ios/RootViewController.mm
             proj.ios_mac/ios/Prefix.pch
             ${APP_UI_RES}
             )
    elseif(MACOSX)
        set(APP_UI_RES
            proj.ios_mac/mac/Icon.icns
            proj.ios_mac/mac/Info.plist
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/mac/main.cpp
             proj.ios_mac/mac/Prefix.pch
             ${APP_UI_RES}
             )
    endif()
    list(APPEND GAME_SOURCE ${common_res_files})
endif()

# mark app complie info and libs info
set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )
if(NOT ANDROID)
    add_executable(${APP_NAME} ${all_code_files})
else()
    add_library(${APP_NAME} SHARED ${all_code_files})
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/platform/android ${ENGINE_BINARY_PATH}/cocos/platform)
    target_link_libraries(${APP_NAME} -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive)
endif()

target_link_libraries(${APP_NAME} cocos2d)
target_include_directories(${APP_NAME}
        PRIVATE Classes
        PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
)

target_include_directories(${APP_NAME} PUBLIC libs/jsoncpp/json)
target_include_directories(${APP_NAME} PUBLIC libs/jsoncpp)
target_include_directories(${APP_NAME} PUBLIC libs/Onigmo)
target_include_directories(${APP_NAME} PUBLIC libs/Onigmo/enc/unicode)
target_include_directories(${APP_NAME} PUBLIC libs/tm-parser/textmate/parser)
target_include_directories(${APP_NAME} PUBLIC libs/tm-parser/textmate/scopes)
target_include_directories(${APP_NAME} PUBLIC libs/tm-parser/textmate/theme)
target_include_directories(${APP_NAME} PUBLIC libs/ashlar)
target_include_directories(${APP_NAME} PUBLIC libs/imgui)

# mark app resources
setup_cocos_app_config(${APP_NAME})
if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    if(MACOSX)
        set_target_properties(${APP_NAME} PROPERTIES
                              MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist"
                              )
    elseif(IOS)
        cocos_pak_xcode(${APP_NAME} INFO_PLIST "iOSBundleInfo.plist.in")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
        set_xcode_property(${APP_NAME} DEVELOPMENT_TEAM "")
        set_xcode_property(${APP_NAME} CODE_SIGN_IDENTITY "iPhone Developer")
    endif()
elseif(WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif()

if(LINUX OR WINDOWS)
    set(APP_RES_DIR "$<TARGET_FILE_DIR:${APP_NAME}>/Resources")
    cocos_copy_target_res(${APP_NAME} COPY_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
endif()
