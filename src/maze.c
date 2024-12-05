#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "../include/maze.h"

#define ROWS 11
#define COLS 20

CellType maze[ROWS][COLS] = 
{
    {WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
    {WALL, POWERUP, POINT, POINT, POINT, WALL, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, WALL, POINT, POINT, POINT, POINT, WALL},
    {WALL, POINT, WALL, WALL, POINT, WALL, POINT, WALL, WALL, WALL, WALL, WALL, WALL, POINT, WALL, POINT, WALL, WALL, POINT, WALL},
    {WALL, POINT, WALL, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, WALL, POINT, WALL},
    {WALL, POINT, WALL, POINT, WALL, WALL, POINT, WALL, WALL, EMPTY, EMPTY, WALL, WALL, POINT, WALL, WALL, POINT, WALL, POINT, WALL},
    {WALL, POINT, POINT, POINT, POINT, POINT, POINT, WALL, EMPTY, EMPTY, EMPTY, EMPTY, WALL, POINT, POINT, POINT, POINT, POINT ,POINT, WALL},
    {WALL, POINT, WALL, POINT, WALL, WALL, POINT, WALL, WALL, WALL, WALL, WALL, WALL, POINT, WALL, WALL, POINT, WALL, POINT, WALL},
    {WALL, POINT, WALL, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, WALL, POINT, WALL},
    {WALL, POINT, WALL, WALL, POINT, WALL, POINT, WALL, WALL, WALL, WALL, WALL, WALL, POINT, WALL, POINT, WALL, WALL, POINT, WALL},
    {WALL, POINT, POINT, POINT, POINT, WALL, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, WALL, POINT, POINT, POINT, POWERUP, WALL},
    {WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL}
};

void display_window(const char* title, int width, int height) {
    // sdl init
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL init error: %s\n", SDL_GetError());
        return;
    }

    // window creation
    SDL_Window* window = SDL_CreateWindow(
        title, 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        width, height, 
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        printf("error creation window: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    // renderer creation
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("error creation renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // texture loading
    SDL_Texture* texture = IMG_LoadTexture(renderer, "assets/corner.png");
    if (!texture) {
        printf("error loading texture: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return;
    }

    // main loop 
    int running = 1;
    SDL_Event event;
    while (running) {
        // event handler
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // texture pos
        SDL_Rect destRect = { 200, 150, 240, 180 }; // x, y, width, height

        // draw texture
        SDL_RenderCopy(renderer, texture, NULL, &destRect);

        // rendering
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // delay ≈ 60 fps
    }

    // resources cleaning
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}