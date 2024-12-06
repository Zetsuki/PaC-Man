#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/maze.h"
#include "../include/render.h"

CellType maze[ROWS][COLS] = 
{
    {CORNER, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, JUNCTION, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, JUNCTION, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, CORNER},
    {MIDDLE_WALL, POWERUP, POINT, POINT, POINT, MIDDLE_WALL, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, MIDDLE_WALL, POINT, POINT, POINT, POINT, MIDDLE_WALL},
    {MIDDLE_WALL, POINT, CORNER, OUTER_WALL, POINT, OUTER_WALL, POINT, OUTER_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, OUTER_WALL, POINT, OUTER_WALL, POINT, OUTER_WALL, CORNER, POINT, MIDDLE_WALL},
    {MIDDLE_WALL, POINT, MIDDLE_WALL, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, MIDDLE_WALL, POINT, MIDDLE_WALL},
    {MIDDLE_WALL, POINT, OUTER_WALL, POINT, OUTER_WALL, OUTER_WALL, POINT, CORNER, OUTER_WALL, EMPTY, EMPTY, OUTER_WALL, CORNER, POINT, OUTER_WALL, OUTER_WALL, POINT, OUTER_WALL, POINT, MIDDLE_WALL},
    {MIDDLE_WALL, POINT, POINT, POINT, POINT, POINT, POINT, MIDDLE_WALL, EMPTY, EMPTY, EMPTY, EMPTY, MIDDLE_WALL, POINT, POINT, POINT, POINT, POINT ,POINT, MIDDLE_WALL},
    {MIDDLE_WALL, POINT, OUTER_WALL, POINT, OUTER_WALL, OUTER_WALL, POINT, CORNER, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, CORNER, POINT, OUTER_WALL, OUTER_WALL, POINT, OUTER_WALL, POINT, MIDDLE_WALL},
    {MIDDLE_WALL, POINT, MIDDLE_WALL, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, MIDDLE_WALL, POINT, MIDDLE_WALL},
    {MIDDLE_WALL, POINT, CORNER, OUTER_WALL, POINT, OUTER_WALL, POINT, OUTER_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, OUTER_WALL, POINT, OUTER_WALL, POINT, OUTER_WALL, CORNER, POINT, MIDDLE_WALL},
    {MIDDLE_WALL, POINT, POINT, POINT, POINT, MIDDLE_WALL, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, MIDDLE_WALL, POINT, POINT, POINT, POWERUP, MIDDLE_WALL},
    {CORNER, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, JUNCTION, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, JUNCTION, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, CORNER}
};

SDL_Texture* maze_cell_textures[7];

void load_all_maze_textures(SDL_Renderer* renderer) {
    maze_cell_textures[EMPTY] = load_texture(renderer, "assets/empty.png");
    maze_cell_textures[MIDDLE_WALL] = load_texture(renderer, "assets/middle_wall.png");
    maze_cell_textures[OUTER_WALL] = load_texture(renderer, "assets/outer_wall.png");
    maze_cell_textures[JUNCTION] = load_texture(renderer, "assets/junction.png");
    maze_cell_textures[CORNER] = load_texture(renderer, "assets/corner.png");
    maze_cell_textures[POINT] = load_texture(renderer, "assets/point.png");
    maze_cell_textures[POWERUP] = load_texture(renderer, "assets/powerup.png");
}

// checks if the current CellType [row][col] needs to be rotated to smooth the texture
void need_rotation(bool* rota, int* deg, CellType maze[ROWS][COLS], int row, int col) {
    CellType current = maze[row][col];

    // default
    *rota = false;
    *deg = 0;

    // adjacent cells, we avoid checking out of bounds cells
    bool up = (row > 0 && maze[row-1][col] != EMPTY && maze[row-1][col] != POINT && maze[row-1][col] != POWERUP);
    bool down = (row < ROWS-1 && maze[row+1][col] != EMPTY && maze[row+1][col] != POINT && maze[row+1][col] != POWERUP);;
    bool left = (col > 0 && maze[row][col-1] != EMPTY && maze[row][col-1] != POINT && maze[row][col-1] != POWERUP);;
    bool right = (col < COLS-1 && maze[row][col+1] != EMPTY && maze[row][col+1] != POINT && maze[row][col+1] != POWERUP);;

    switch (current) {
        case MIDDLE_WALL:
            // horizontal by default, so we check for vertical
            if (up && down && !left && !right) { *rota = true; *deg = 90; }
            break;
        case OUTER_WALL:
            // horizontal and extremity to the right by default, so we check for the three other possible extremity orientation
            if (up) { *rota = true; *deg = 90; }
            if (right) { *rota = true; *deg = 180; }
            if (down) { *rota = true; *deg = 270; }
            break;
        case JUNCTION:
            // horizontal and connects to the left, bottom and right by default, checking the three others possible connection
            if (down && left && up) { *rota = true; *deg = 90; }
            if (left && up && right) { *rota = true; *deg = 180; }
            if (up && right && down) { *rota = true; *deg = 270; }
            break;
        case CORNER:
            // connects bottom and right by default, checking the three others connections
            if (down && left) { *rota = true; *deg = 90; }
            if (left && up) { *rota = true; *deg = 180; }
            if (up && right) { *rota = true; *deg = 270; }
            break;
        default:
            break;
    }
};

void render_maze(SDL_Renderer* renderer, int* width, int* height) {
    int tile_width = 32;
    int tile_height = 32;

    // calculation of scale factor and offsets 
    float scale_x = (float)*width / (COLS * tile_width);
    float scale_y = (float)*height / (ROWS * tile_height);
    float scale = scale_x < scale_y ? scale_x : scale_y; // aspect ratio
    scale *= 0.85f; // 85%

    int offset_x = (*width - (COLS * tile_width * scale)) / 2;
    int offset_y = (*height - (ROWS * tile_height * scale)) / 2;

    bool needs_rotation;
    int rotation_degrees;
    
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            SDL_Rect destRect = { 
                offset_x + col * tile_width * scale,
                offset_y + row * tile_height * scale,
                tile_width * scale,
                tile_height * scale
            };

            // rotate texture if needed
            need_rotation(&needs_rotation, &rotation_degrees, maze, row, col);
            if (needs_rotation) {
                SDL_RenderCopyEx(renderer, maze_cell_textures[maze[row][col]], NULL, &destRect, rotation_degrees, NULL, SDL_FLIP_NONE);
            } else {
                SDL_RenderCopy(renderer, maze_cell_textures[maze[row][col]], NULL, &destRect);
            }
        }
    }
}

void cleanup_maze(SDL_Window* window, SDL_Renderer* renderer) {
    for (int i = 0; i < 7; ++i) {
        SDL_DestroyTexture(maze_cell_textures[i]);
    }
}