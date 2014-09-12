#include <iostream>
#include <SDL2/SDL.h>

class Game {
    
public:
    Game() {}
    ~Game() {}
    
    bool init (const char* title, int xpos, int ypos, int width, int height, int flags);
    void loadImage(std::string name);
    void renderTexture(SDL_Texture* tex, SDL_Renderer* renderer, int x, int y);
    void render();
    void update();
    void handleEvents();
    void clean();
    bool running();
    
private:
    bool gameRunning;
    SDL_Window* mainWindow;
    SDL_Renderer* mainRenderer;
    SDL_Texture* mainTexture;
    int m_x = 1;
    int m_y = 1;
};

