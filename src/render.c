#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "../include/pacman.h"
#include "../include/utils.h"
#include "../include/render.h"

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

void initialize_render(RenderState* render, Pacman* pacman)
{
    SDL_Window* window;
    SDL_Renderer* renderer;

    initialize_SDL(&window, &renderer, render->width, render->height);
    render->window = window;
    render->renderer = renderer;
    load_all_textures(render, pacman);
}

SDL_Texture* load_texture(SDL_Renderer* renderer, const char* file_path) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, file_path);
    if (!texture) {
        printf("Error loading texture: %s\n", IMG_GetError());
    }
    return texture;
}

// load all relevent textures for the game
void load_all_textures(RenderState* render, Pacman* pacman) {
    render->maze_cell_textures[EMPTY] = load_texture(render->renderer, "assets/empty.png");
    render->maze_cell_textures[MIDDLE_WALL] = load_texture(render->renderer, "assets/middle_wall.png");
    render->maze_cell_textures[OUTER_WALL] = load_texture(render->renderer, "assets/outer_wall.png");
    render->maze_cell_textures[JUNCTION] = load_texture(render->renderer, "assets/junction.png");
    render->maze_cell_textures[CORNER] = load_texture(render->renderer, "assets/corner.png");
    render->maze_cell_textures[WARP] = load_texture(render->renderer, "assets/warp.png");
    render->maze_cell_textures[POINT] = load_texture(render->renderer, "assets/point.png");
    render->maze_cell_textures[POWERUP] = load_texture(render->renderer, "assets/powerup.png");

    render->pacman_texture = load_texture(render->renderer, "assets/pacman.png");
    render->angry_pacman_texture = load_texture(render->renderer, "assets/angry_pacman.png");
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

// get scales and offsets so the game is scalable to the window size
void calculate_scale_and_offsets(RenderState* render, float* scale, int* offset_x, int* offset_y) {
    float scale_x = (float)*render->width / (COLS * TILE_WIDTH);
    float scale_y = (float)*render->height / (ROWS * TILE_HEIGHT);
    *scale = scale_x < scale_y ? scale_x : scale_y;
    *scale *= 0.85f; // 85%

    *offset_x = (*render->width - (COLS * TILE_WIDTH * (*scale))) / 2;
    *offset_y = (*render->height - (ROWS * TILE_HEIGHT * (*scale))) / 2;
}

// texture swapping when pacman eats pellets
void swap_texture(Pacman* pacman) {
    if (maze[pacman->y][pacman->x] != WARP) {
        maze[pacman->y][pacman->x] = EMPTY;
    }
}

// render texture using scale and offsets
void render_scaled_texture(RenderState* render, SDL_Texture* texture, int x, int y, float scale, int offset_x, int offset_y, int rotation_degrees, bool needs_rotation, SDL_RendererFlip flip) {
    SDL_Rect destRect = {
        offset_x + x * TILE_WIDTH * scale,
        offset_y + y * TILE_HEIGHT * scale,
        TILE_WIDTH * scale,
        TILE_HEIGHT * scale
    };

    if (needs_rotation) {
        SDL_RenderCopyEx(render->renderer, texture, NULL, &destRect, rotation_degrees, NULL, flip);
    } else {
        SDL_RenderCopy(render->renderer, texture, NULL, &destRect);
    }
}

void render_maze(RenderState* render) {
    float scale;
    int offset_x, offset_y;
    
    calculate_scale_and_offsets(render, &scale, &offset_x, &offset_y);

    bool needs_rotation;
    int rotation_degrees;

    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            need_rotation(&needs_rotation, &rotation_degrees, maze, row, col);
            render_scaled_texture(render, render->maze_cell_textures[maze[row][col]], col, row, scale, offset_x, offset_y, rotation_degrees, needs_rotation, SDL_FLIP_NONE);
        }
    }
}

void render_pacman(RenderState* render, Pacman* pacman) {
    float scale;
    int offset_x, offset_y;

    calculate_scale_and_offsets(render, &scale, &offset_x, &offset_y);

    SDL_Texture* current_texture = pacman->powered_up ? render->angry_pacman_texture : render->pacman_texture;
    int rotation_degrees = 0;
    bool needs_rotation = false;
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    switch (pacman->dir) {
        case UP:
            rotation_degrees = 270;
            needs_rotation = true;
            flip =  SDL_FLIP_NONE;
            break;
        case DOWN:
            rotation_degrees = 90;
            needs_rotation = true;
            flip = SDL_FLIP_VERTICAL;
            break;
        case LEFT:
            rotation_degrees = 180;
            needs_rotation = true;
            flip = SDL_FLIP_VERTICAL;
            break;
        case RIGHT:
            needs_rotation = false;
            flip = SDL_FLIP_NONE;
            break;
        default:
            needs_rotation = false;
            flip = SDL_FLIP_NONE;
            break;
    }
    render_scaled_texture(render, current_texture, pacman->x, pacman->y, scale, offset_x, offset_y, rotation_degrees, needs_rotation, flip);
}

void render(RenderState* render, Pacman* pacman) {
    SDL_SetRenderDrawColor(render->renderer, 0, 0, 0, 255);
    SDL_RenderClear(render->renderer);

    swap_texture(pacman);
    render_maze(render);
    render_pacman(render, pacman);

    SDL_RenderPresent(render->renderer);
    SDL_Delay(300);
}

void cleanup(RenderState* render, Pacman* pacman) {
    for (int i = 0; i < 7; ++i) {
        SDL_DestroyTexture(render->maze_cell_textures[i]);
    }
    SDL_DestroyTexture(render->pacman_texture);
    SDL_DestroyTexture(render->angry_pacman_texture);
    SDL_DestroyRenderer(render->renderer);
    SDL_DestroyWindow(render->window);
    IMG_Quit();
    SDL_Quit();
}