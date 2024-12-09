#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../include/pacman.h"
#include "../include/maze.h"
#include "../include/render.h"
#include "../include/config.h"

bool is_wall(CellType cell) {
    return cell == MIDDLE_WALL || cell == OUTER_WALL || cell == CORNER || cell == JUNCTION;
}

void initialize_pacman(Pacman* pacman) {
    pacman->x = PACMAN_X;
    pacman->y = PACMAN_Y;
    pacman->dir = NONE;
    pacman->next_dir = NONE;
    pacman->texture = NULL;
    pacman->powered_up_texture = NULL;
    pacman->powered_up = false;
    pacman->powered_up_time_left = 0;
}

void handle_pacman_event(SDL_Event* event, Pacman* pacman) {
    if (event->type == SDL_KEYDOWN) {
        Direction new_dir = NONE;
        switch (event->key.keysym.sym) {
            case SDLK_w:
            case SDLK_z:
            case SDLK_UP:
                new_dir = UP;
                break;
            case SDLK_s:
            case SDLK_DOWN:
                new_dir = DOWN;
                break;
            case SDLK_a:
            case SDLK_q:
            case SDLK_LEFT:
                new_dir = LEFT;
                break;
            case SDLK_d:
            case SDLK_RIGHT:
                new_dir = RIGHT;
                break;
        }
        pacman->next_dir = new_dir;
    }
}

void update_pacman(Pacman* pacman) {
    int new_x = pacman->x;
    int new_y = pacman->y;

    if (pacman->next_dir != NONE) {
        switch (pacman->next_dir) {
            case UP:
                if (pacman->y > 0 && !is_wall(maze[pacman->y - 1][pacman->x])) {
                    pacman->dir = UP;
                }
                break;
            case DOWN:
                if (pacman->y < ROWS - 1 && !is_wall(maze[pacman->y + 1][pacman->x])) {
                    pacman->dir = DOWN;
                }
                break;
            case LEFT:
                if (pacman->x > 0 && !is_wall(maze[pacman->y][pacman->x - 1])) {
                    pacman->dir = LEFT;
                }
                break;
            case RIGHT:
                if (pacman->x < COLS - 1 && !is_wall(maze[pacman->y][pacman->x + 1])) {
                    pacman->dir = RIGHT;
                }
                break;
            default:
                break;
        }
    }

    switch (pacman->dir) {
        case UP:
            new_y -= 1;
            break;
        case DOWN:
            new_y += 1;
            break;
        case LEFT:
            if (pacman->x > 0 && maze[pacman->y][pacman->x - 1] != WARP) {
                new_x -= 1; 
            } else if (maze[pacman->y][pacman->x - 1] == WARP) {
                new_x = COLS - 1;
            }
            break;
        case RIGHT:
            if (pacman->x < COLS - 1 && maze[pacman->y][pacman->x + 1] != WARP) {
                new_x += 1;
            } else if (maze[pacman->y][pacman->x + 1] == WARP) {
                new_x = 0; 
            }
            break;
        default:
            break;
    }

    if (new_x >= 0 && new_x < COLS && new_y >= 0 && new_y < ROWS && !is_wall(maze[new_y][new_x])) {
        if (maze[pacman->y][pacman->x] != WARP) {
            maze[pacman->y][pacman->x] = EMPTY;
        }
        pacman->x = new_x;
        pacman->y = new_y;

        if (maze[new_y][new_x] == POWERUP) {
            pacman->powered_up = true;
            pacman->powered_up_time_left = 20;
        }
    }

    if (pacman->powered_up_time_left == 0) {
        pacman->powered_up = false;
    }
    else {
        pacman->powered_up_time_left -= 1;
    }

}


void render_pacman(SDL_Renderer* renderer, Pacman* pacman, unsigned int* width, unsigned int* height) {
    int tile_width = 32;
    int tile_height = 32;

    float scale_x = (float)*width / (COLS * tile_width);
    float scale_y = (float)*height / (ROWS * tile_height);
    float scale = scale_x < scale_y ? scale_x : scale_y;
    scale *= 0.85f;

    int offset_x = (*width - (COLS * tile_width * scale)) / 2;
    int offset_y = (*height - (ROWS * tile_height * scale)) / 2;

    SDL_Rect destRect = {
        offset_x + pacman->x * tile_width * scale,
        offset_y + pacman->y * tile_height * scale,
        tile_width * scale,
        tile_height * scale
    };

    SDL_Texture* current_texture = pacman->powered_up ? pacman->powered_up_texture : pacman->texture; 

    switch (pacman->dir) {
        case UP:
            SDL_RenderCopyEx(renderer, current_texture, NULL, &destRect, 270, NULL, SDL_FLIP_VERTICAL);
            break;
        case DOWN:
            SDL_RenderCopyEx(renderer, current_texture, NULL, &destRect, 90, NULL, SDL_FLIP_VERTICAL);
            break;
        case LEFT:
            SDL_RenderCopyEx(renderer, current_texture, NULL, &destRect, 180, NULL, SDL_FLIP_VERTICAL);
            break;
        case RIGHT:
            SDL_RenderCopy(renderer, current_texture, NULL, &destRect);
            break;
        default:
            SDL_RenderCopy(renderer, current_texture, NULL, &destRect);
            break;
    }
}

void cleanup_pacman(Pacman* pacman) {
    SDL_DestroyTexture(pacman->texture);
    SDL_DestroyTexture(pacman->powered_up_texture);
}
