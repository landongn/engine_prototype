//
//  Game.cpp
//  sdl-hello
//
//  Created by thock on 8/9/14.
//  Copyright (c) 2014 landon. All rights reserved.
//

#include "Game.h"
#include "../jsbinding/bindings.h"
#include "../file/file.h"
#include <SDL2/SDL.h>

static const uint8_t SUCCESS = 0;

bool Game::init(const char* title, int xpos, int ypos, int width, int height, int flags) {
    
    if(SDL_Init(SDL_INIT_EVERYTHING) != SUCCESS) {
        std::cout << "SDL_Init failure! " << SDL_GetError() << std::endl;
        return false;
    }
    
    mainWindow = SDL_CreateWindow(title, xpos, ypos, height, width, flags);
    if(!mainWindow) {
        std::cout << "SDL_CreateWindow failure! " << SDL_GetError() << std::endl;
        return false;
    }
    
    mainRenderer = SDL_CreateRenderer(mainWindow, -1, 0);
    if (!mainRenderer) {
        std::cout << "SDL_CreateRenderer failure! " << SDL_GetError() << std::endl;
        return false;
    }
    std::cout << "Renderer created successfully!" << std::endl;
    
    if (SUCCESS != SDL_SetRenderDrawColor(mainRenderer, 255, 255, 255, 255)) {
        std::cout << "SDL_SetRenderDrawColor failure! " << SDL_GetError() << std::endl;
    }
    
    std::cout << "Game running!" << std::endl;
    
    doBindings(this);

    gameRunning = true;
    
    return true;
}

void Game::loadImage(std::string name) {
    std::string imagePath = file::GetPath(name);
    
    std::cout << "Location is" << imagePath.c_str() << std::endl;
    
    SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());
    if (!bmp) {
        std::cout << "Failed to create image. " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(mainRenderer);
        SDL_DestroyWindow(mainWindow);
        SDL_Quit();
    }

    mainTexture = SDL_CreateTextureFromSurface(mainRenderer, bmp);
    SDL_FreeSurface(bmp);
    if (!mainTexture) {
        std::cout << "Failed to create texture. " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(mainRenderer);
        SDL_DestroyWindow(mainWindow);
        SDL_Quit();
    }
    
    std::cout << "Created texture successfully." << std::endl;
}

void Game::renderTexture(SDL_Texture *tex, SDL_Renderer *renderer, int x, int y) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    SDL_QueryTexture(tex, NULL, NULL, &rect.w, &rect.h);
    SDL_RenderCopy(renderer, tex, NULL, &rect);
}

void Game::render() {
    if (gameRunning) {
        SDL_RenderClear(mainRenderer);
    
        if (mainTexture) {
            renderTexture(mainTexture, mainRenderer, m_x, m_y);
        }
    
        SDL_RenderPresent(mainRenderer);
    }
}

bool Game::running() {
    return gameRunning;
}

void Game::update() {

}

void Game::clean() {
    std::cout << "cleaning game" << std::endl;
    SDL_DestroyWindow(mainWindow);
    SDL_DestroyRenderer(mainRenderer);
    SDL_Quit();
}

void Game::handleEvents() {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                gameRunning = false;
                break;
            case SDL_KEYDOWN:
                gameRunning = false;
                break;
            default:
                break;
        }
    }
}
