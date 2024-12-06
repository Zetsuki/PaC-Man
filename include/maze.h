#ifndef maze_H
#define maze_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define ROWS 11
#define COLS 20

typedef enum {
    EMPTY, 
    MIDDLE_WALL,
    OUTER_WALL,
    JUNCTION,
    CORNER, 
    POINT,
    POWERUP
} CellType;

extern CellType maze[ROWS][COLS];

void load_all_maze_textures(SDL_Renderer* renderer);
void need_rotation(bool* rota, int* deg, CellType maze[ROWS][COLS], int row, int col);
void render_maze(SDL_Renderer* renderer, int* width, int* height);
void cleanup_maze(SDL_Window* window, SDL_Renderer* renderer);

#endif