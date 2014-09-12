//
//  bindings.cpp
//  sdl-hello
//
//  Created by Dan Ristic on 9/8/14.
//  Copyright (c) 2014 landon. All rights reserved.
//


#include "bindings.h"
#include "file.h"
#include "JavaScriptCore/JSValueRef.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>

JSClassRef ConsoleClass()
{
    static JSStaticFunction staticFunctions[] = {
        { "log", console_log, kJSPropertyAttributeNone }
    };
    
    static JSClassDefinition classDefinition = {
        0, kJSClassAttributeNone, "console", 0, 0, staticFunctions,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    
    static JSClassRef consoleClass = JSClassCreate(&classDefinition);
    return consoleClass;
}

/**
 * The callback from JavaScriptCore.  We told JSC to call this function
 * whenever it sees "console.log".
 */
static JSValueRef console_log(JSContextRef ctx, JSObjectRef /*function*/, JSObjectRef thisObject, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception)
{
    if (!JSValueIsObjectOfClass(ctx, thisObject, ConsoleClass()))
        return JSValueMakeUndefined(ctx);
    
    if (argumentCount < 1)
        return JSValueMakeUndefined(ctx);
    
    // Convert the result into a string for display.
    if (!JSValueIsString(ctx, arguments[0]))
        return JSValueMakeUndefined(ctx);
    
    JSStringRef temp = JSValueToStringCopy (ctx, arguments[0], exception);
    if (exception && *exception)
        return JSValueMakeUndefined(ctx);
    
    char otherStr[1024];
    JSStringGetUTF8CString(temp, otherStr, sizeof(char[1024]));
    printf("Got console log %s\n", otherStr);
    
    JSStringRelease(temp);
    
    return JSValueMakeUndefined(ctx);
}

JSClassRef EngineClass()
{
    static JSStaticFunction staticFunctions[] = {
        { "createImage", create_image, kJSPropertyAttributeNone }
    };
    
    static JSClassDefinition classDefinition = {
        0, kJSClassAttributeNone, "engine", 0, 0, staticFunctions,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    
    static JSClassRef engineClass = JSClassCreate(&classDefinition);
    return engineClass;
}

/**
 * The callback from JavaScriptCore.  We told JSC to call this function
 * whenever it sees "console.log".
 */
static JSValueRef create_image(JSContextRef ctx, JSObjectRef /*function*/, JSObjectRef thisObject, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception)
{
    std::cout << "CREATING" << std::endl;
    if (!JSValueIsObjectOfClass(ctx, thisObject, EngineClass()))
        return JSValueMakeUndefined(ctx);
    
    if (argumentCount < 1)
        return JSValueMakeUndefined(ctx);
    
    // Convert the result into a string for display.
    if (!JSValueIsString(ctx, arguments[0]))
        return JSValueMakeUndefined(ctx);
    
    JSStringRef temp = JSValueToStringCopy (ctx, arguments[0], exception);
    if (exception && *exception)
        return JSValueMakeUndefined(ctx);
    
    Game* game = (Game*)JSObjectGetPrivate(thisObject);
    assert(game);
    
    char otherStr[1024];
    JSStringGetUTF8CString(temp, otherStr, sizeof(char[1024]));
    printf("Engine create_image %s\n", otherStr);
    
    game->loadImage(string(otherStr));
    
    JSStringRelease(temp);
    
    return JSValueMakeUndefined(ctx);
}

string loadData(Game* game) {
    string line;
    string result = "";
    string input = file::GetPath("myscript.js");
    std::ifstream openFile;
    openFile.open(input.c_str(), std::ios::in);
    
    if (openFile.is_open()) {
        while (std::getline(openFile, line))
        {
            result += line;
        }
        
        openFile.close();
    } else {
        std::cout << "[ERROR] Unable to open file." << std::endl;
        return "";
    }
    
    return result;
}

void doBindings(Game* game) {
    JSGlobalContextRef ctx = JSGlobalContextCreate(NULL);
    
    JSObjectRef global = JSContextGetGlobalObject(ctx);
    assert(global);
    
    JSClassRef consoleClass = ConsoleClass();
    assert(consoleClass);
    
    JSObjectRef scriptObj = JSObjectMake(ctx, consoleClass, NULL);
    assert(scriptObj);
    
    JSStringRef consoleName = JSStringCreateWithUTF8CString("console");
    JSValueRef* except = 0;
    JSObjectSetProperty(ctx, global, consoleName, scriptObj, kJSPropertyAttributeNone, except);
    
    JSClassRef engineClass = EngineClass();
    assert(engineClass);
    JSObjectRef engineObj = JSObjectMake(ctx, engineClass, reinterpret_cast<void*>(game));
    assert(engineObj);
    
    JSStringRef engineName = JSStringCreateWithUTF8CString("engine");
    JSObjectSetProperty(ctx, global, engineName, engineObj, kJSPropertyAttributeNone, NULL);
    
    string functionString = loadData(game);
    JSStringRef jsString = JSStringCreateWithUTF8CString(functionString.c_str());
    
    std::cout << "Executing JS code..." << std::endl;
    
    JSValueRef exception = NULL;
    JSValueRef ref = JSEvaluateScript(ctx, jsString, NULL, NULL, 1, &exception);
    
    if (exception) {
        JSStringRef str = JSValueToStringCopy(ctx, exception, NULL);
        
        char myStr[1024];
        JSStringGetUTF8CString(str, myStr, sizeof(char[1024]));
        
        std::cout << "Error hit" << myStr << std::endl;
        
        JSStringRelease(str);
    }
    
    JSStringRef strRef = JSValueToStringCopy(ctx, ref, NULL);
    char otherStr[1024];
    JSStringGetUTF8CString(strRef, otherStr, sizeof(char[1024]));
    
    JSStringRelease(strRef);
}

