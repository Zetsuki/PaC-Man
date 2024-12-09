#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include "../include/pacman.h"

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    unsigned int* width;
    unsigned int* height;
} RenderState;

void initialize_render(RenderState* render, Pacman* pacman);
void initialize_SDL(SDL_Window** window, SDL_Renderer** renderer, unsigned int* width, unsigned int* height);
SDL_Texture* load_texture(SDL_Renderer* renderer, const char* file_path);
void load_all_textures(SDL_Renderer* renderer, Pacman* pacman);
void cleanup(SDL_Window* window, SDL_Renderer* renderer, Pacman* pacman);
void render_loop(SDL_Renderer* renderer, SDL_Window* window, unsigned int* width, unsigned int* height, Pacman* pacman);
void render(RenderState* render, Pacman* pacman);

#endif