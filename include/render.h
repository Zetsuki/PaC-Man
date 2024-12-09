#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include "../include/pacman.h"

void initialize_SDL(SDL_Window** window, SDL_Renderer** renderer, const char* title, int* width, int* height);
SDL_Texture* load_texture(SDL_Renderer* renderer, const char* file_path);
void load_all_textures(SDL_Renderer* renderer);
void cleanup(SDL_Window* window, SDL_Renderer* renderer, Pacman* pacman);
void render_loop(SDL_Renderer* renderer, SDL_Window* window, int* width, int* height, Pacman* pacman);
void render(const char* title, int* width, int* height, Pacman* pacman);

#endif