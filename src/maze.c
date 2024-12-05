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

    // adjacent cells, we avoir checking out of bounds cells
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
            break;
        case JUNCTION:
            break;
        case CORNER:
            break;
        default:
            break;
    }
};

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
        }
        
        // background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        int smoothed_maze = 0;
        bool needs_rotation;
        int rotation_degrees;

        // iterate through array, to render all textures
        for (int row = 0; row < ROWS; ++row) {
            for (int col = 0; col < COLS; ++col) {
                // textures are not yet smoothed via rotation
                if (smoothed_maze == 0) {
                    need_rotation(&needs_rotation, &rotation_degrees, maze, row, col);

                    if (needs_rotation) {
                        SDL_Rect destRect = { col * 32, row * 32, 32, 32 }; //x, y, width, height
                        SDL_RenderCopyEx(renderer, textures[maze[row][col]], NULL, &destRect, rotation_degrees, NULL, SDL_FLIP_NONE);
                    }
                    else {
                        SDL_Rect destRect = { col * 32, row * 32, 32, 32 };
                        SDL_RenderCopy(renderer, textures[maze[row][col]], NULL, &destRect);
                    }
                }
                // textures are already smoothed
                else {
                    SDL_Rect destRect = { col * 32, row * 32, 32, 32 };
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