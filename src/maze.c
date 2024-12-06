#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "../include/maze.h"

#define ROWS 11
#define COLS 20

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

SDL_Texture* textures[7];


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
            if (up && down && !left && !right) {
                *rota = true;
                *deg = 90;
            }
            break;
        case OUTER_WALL:
            // horizontal and extremity to the right by default, so we check for the three other possible extremity orientation
            if (up) {
                *rota = true;
                *deg = 90;
            }
            if (right) {
                *rota = true;
                *deg = 180;
            }
            if (down) {
                *rota = true;
                *deg = 270;
            }
            break;
        case JUNCTION:
            // horizontal and connects to the left, bottom and right by default, checking the three others possible connection
            if (down && left && up) {
                *rota = true;
                *deg = 90;
            }
            if (left && up && right) {
                *rota = true;
                *deg = 180;
            }
            if (up && right && down) {
                *rota = true;
                *deg = 270;
            }
            break;
        case CORNER:
            // connects bottom and right by default, checking the three others connections
            if (down && left) {
                *rota = true;
                *deg = 90;
            }
            if (left && up) {
                *rota = true;
                *deg = 180;
            }
            if (up && right) {
                *rota = true;
                *deg = 270;
            }
            break;
        default:
            break;
    }
};

void display_window(const char* title, int* width, int* height) {
    // sdl init
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL init error: %s\n", SDL_GetError());
        return;
    }

    // window creation
    SDL_Window* window = SDL_CreateWindow(
        title, 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        *width, *height, 
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
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

    // load textures for each maze element
    textures[EMPTY] = IMG_LoadTexture(renderer, "assets/empty.png");
    textures[MIDDLE_WALL] = IMG_LoadTexture(renderer, "assets/middle_wall.png");
    textures[OUTER_WALL] = IMG_LoadTexture(renderer, "assets/outer_wall.png");
    textures[JUNCTION] = IMG_LoadTexture(renderer, "assets/junction.png");
    textures[CORNER] = IMG_LoadTexture(renderer, "assets/corner.png");
    textures[POINT] = IMG_LoadTexture(renderer, "assets/point.png");
    textures[POWERUP] = IMG_LoadTexture(renderer, "assets/powerup.png");
    
    for (int i = 0; i < 7; ++i) {
        if (!textures[i]) {
            printf("Error loading texture %d: %s\n", i, IMG_GetError());
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            IMG_Quit();
            SDL_Quit();
            return;
        }
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

            // window resizing
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                *width = event.window.data1;
                *height = event.window.data2;
            }
        }
        
        // background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        int smoothed_maze = 0; // to prevent unnecessary checks on already smoothed maze textures
        bool needs_rotation;
        int rotation_degrees;

        // calculation of scale factor and offsets 
        int tile_width = 32;
        int tile_height = 32;

        float scale_x = (float)*width / (COLS * tile_width);
        float scale_y = (float)*height / (ROWS * tile_height);
        float scale = scale_x < scale_y ? scale_x : scale_y; // aspect ratio
        scale *= 0.8f; // 80%

        int offset_x = (*width - (COLS * tile_width * scale)) / 2;
        int offset_y = (*height - (ROWS * tile_height * scale)) / 2;

        // iterate through array, to render all textures
        for (int row = 0; row < ROWS; ++row) {
            for (int col = 0; col < COLS; ++col) {
                SDL_Rect destRect = { 
                    offset_x + col * tile_width * scale,    // x
                    offset_y + row * tile_height * scale,   // y
                    tile_width * scale,                     // width
                    tile_height * scale,                    // height
                };

                // textures are not yet smoothed
                if (smoothed_maze == 0) {
                    need_rotation(&needs_rotation, &rotation_degrees, maze, row, col);

                    if (needs_rotation) {
                        SDL_RenderCopyEx(renderer, textures[maze[row][col]], NULL, &destRect, rotation_degrees, NULL, SDL_FLIP_NONE);
                    }
                    else {
                        SDL_RenderCopy(renderer, textures[maze[row][col]], NULL, &destRect);
                    }
                }
                // textures are already smoothed
                else {
                    
                    SDL_RenderCopy(renderer, textures[maze[row][col]], NULL, &destRect);
                } 
            }
        }
        smoothed_maze = 1;

        // rendering
        SDL_RenderPresent(renderer);
    
        SDL_Delay(16); // delay ≈ 60 fps
    }

    // resources cleaning
    for (int i = 0; i < 7; ++i) {
        SDL_DestroyTexture(textures[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}