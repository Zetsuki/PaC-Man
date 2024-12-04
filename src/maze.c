#include <SDL2/SDL.h>
#include <stdio.h>
#include "../include/maze.h"

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

        // red rectangle
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect rect = { width / 4, height / 4, width / 2, height / 2 };
        SDL_RenderFillRect(renderer, &rect);

        // rendering
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // delay ≈ 60 fps
    }

    // asset cleaning
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}