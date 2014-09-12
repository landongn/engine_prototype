//
//  main.cpp
//  prototype
//
//  Created by thock on 9/10/14.
//  Copyright (c) 2014 onlyfunthings. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "src/file/file.h"
#include "src/core/Game.h"

enum ExitCodes {
    SUCCESS = 0,
    ERR_OH_CRAP_NO_MEMORY = 1,
    ERR_PHUQ_INIT_FAILED = 2
};

int main(int argc, const char * argv[]) {
    Game* _game(new Game());
    if(!_game){
        std::cout << "Could not create Game object." << std::endl;
        return ERR_OH_CRAP_NO_MEMORY;
    }
    
    if(!_game->init("Chapter 1", 100, 100, 720, 1280, 0)){
        std::cout << "Could not initialize Game object." << std::endl;
        return ERR_PHUQ_INIT_FAILED;
    }
	
    while(_game->running()) {
        _game->handleEvents();
        _game->update();
        _game->render();
    }
    
    _game->clean();
    
    return SUCCESS;
}
