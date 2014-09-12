//
//  file.cpp
//  sdl-hello
//
//  Created by Dan Ristic on 9/10/14.
//  Copyright (c) 2014 landon. All rights reserved.
//

#include "file.h"
#include <SDL2/SDL.h>

namespace file {
    char* initialize();
}

char* _data_path = file::initialize();

char* file::initialize() {
    // Grab the base resource path
    char* data_path;
    char *_base_path = SDL_GetBasePath();

    if (_base_path) {
        data_path = SDL_strdup(_base_path);
        SDL_free(_base_path);
    } else {
        data_path = SDL_strdup("./");
    }
    
    return data_path;
}

std::string file::GetPath(std::string name) {
    std::string result;
    
    result = std::string(_data_path) + "Resources/" + name;
    
    return result;
}