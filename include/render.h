#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include "../include/pacman.h"
#include "../include/utils.h"

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    unsigned int* width; // window size
    unsigned int* height; //
    SDL_Texture* maze_cell_textures[8];
    SDL_Texture* pacman_texture;
    SDL_Texture* angry_pacman_texture; // texture when pacman eats a power pellet
} RenderState;

void initialize_SDL(SDL_Window** window, SDL_Renderer** renderer, unsigned int* width, unsigned int* height);
void initialize_render(RenderState* render, Pacman* pacman);

SDL_Texture* load_texture(SDL_Renderer* renderer, const char* file_path);
void load_all_textures(RenderState* render, Pacman* pacman); // load all textures used during the game

// checks if the current CellType [row][col] needs to be rotated to smooth out the texture
void need_rotation(bool* rota, int* deg, CellType maze[ROWS][COLS], int row, int col);
// get scale and offsets so the game is scalable to the window size
void calculate_scale_and_offsets(RenderState* render, float* scale, int* offset_x, int* offset_y);

// texture swapping when pacman eats pellets
void swap_texture(Pacman* pacman);

void render_scaled_texture(RenderState* render, SDL_Texture* texture, int x, int y, float scale, int offset_x, int offset_y, int rotation_degrees, bool needs_rotation, SDL_RendererFlip flip);
void render_maze(RenderState* render);
void render_pacman(RenderState* render, Pacman* pacman);
void render(RenderState* render, Pacman* pacman);

void cleanup(RenderState* render, Pacman* pacman);

#endif