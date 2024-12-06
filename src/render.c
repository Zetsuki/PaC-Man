#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "../include/render.h"
#include "../include/maze.h"
#include "../include/pacman.h"


void initialize_SDL(SDL_Window** window, SDL_Renderer** renderer, const char* title, int* width, int* height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL init error: %s\n", SDL_GetError());
    }

    *window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, *width, *height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!*window) {
        printf("Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
    }
}

SDL_Texture* load_texture(SDL_Renderer* renderer, const char* file_path) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, file_path);
    if (!texture) {
        printf("Error loading texture: %s\n", IMG_GetError());
    }
    return texture;
}

void load_all_textures(SDL_Renderer* renderer) {
    load_all_maze_textures(renderer);
}

void cleanup(SDL_Window* window, SDL_Renderer* renderer, Pacman* pacman) {
    cleanup_maze (window, renderer);
    cleanup_pacman(pacman);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void render_loop(SDL_Renderer* renderer, SDL_Window* window, int* width, int* height, Pacman* pacman) {
    int running = 1;
    SDL_Event event;

    initialize_pacman(renderer, pacman);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            // window resizing
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                *width = event.window.data1;
                *height = event.window.data2;
            }
            handle_pacman_event(&event, pacman);
        }

        update_pacman(pacman, maze);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_maze(renderer, width, height);
        render_pacman(renderer, pacman, width, height);

        SDL_RenderPresent(renderer);
        SDL_Delay(450);
    }
}

void render(const char* title, int* width, int* height, Pacman* pacman) {
    SDL_Window* window;
    SDL_Renderer* renderer;

    initialize_SDL(&window, &renderer, title, width, height);
    load_all_textures(renderer);
    render_loop(renderer, window, width, height, pacman);
    cleanup(window, renderer, pacman);
}