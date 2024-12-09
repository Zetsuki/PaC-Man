#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "../include/render.h"
#include "../include/maze.h"
#include "../include/pacman.h"

void initialize_render(RenderState* render, Pacman* pacman)
{
    SDL_Window* window;
    SDL_Renderer* renderer;

    initialize_SDL(&window, &renderer, render->width, render->height);
    render->window = window;
    render->renderer = renderer;
    load_all_textures(renderer, pacman);
}

void initialize_SDL(SDL_Window** window, SDL_Renderer** renderer, unsigned int* width, unsigned int* height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL init error: %s\n", SDL_GetError());
    }

    *window = SDL_CreateWindow("PaC-Man", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, *width, *height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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

void load_all_textures(SDL_Renderer* renderer, Pacman* pacman) {
    load_all_maze_textures(renderer);
    load_all_pac_textures(renderer, pacman);
}

void cleanup(SDL_Window* window, SDL_Renderer* renderer, Pacman* pacman) {
    cleanup_maze (window, renderer);
    cleanup_pacman(pacman);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void render_loop(SDL_Renderer* renderer, SDL_Window* window, unsigned int* width, unsigned int* height, Pacman* pacman) {
    int running = 1;
    SDL_Event event;

    initialize_pacman(pacman);

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

        update_pacman(pacman);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_maze(renderer, width, height);
        render_pacman(renderer, pacman, width, height);

        SDL_RenderPresent(renderer);
        SDL_Delay(300);
        
    }
}

void render(RenderState* render, Pacman* pacman) {
    SDL_SetRenderDrawColor(render->renderer, 0, 0, 0, 255);
    SDL_RenderClear(render->renderer);
    render_maze(render->renderer, render->width, render->height);
    render_pacman(render->renderer, pacman, render->width, render->height);

    SDL_RenderPresent(render->renderer);
    SDL_Delay(300);
}