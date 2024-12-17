#include <stdio.h>
#include "../include/pacman.h"
#include "../include/utils.h"

void initialize_pacman(Pacman* pacman) {
    pacman->x = PACMAN_X;
    pacman->y = PACMAN_Y;
    pacman->dir = NONE;
    pacman->next_dir = NONE;
    pacman->powered_up = false;
    pacman->powered_up_time_left = 0;
    pacman->life = 3;
}

// handling user inputs to move (arrows, WASD and ZQSD)
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

// handle pacman movement, postion and power up
void update_pacman(Pacman* pacman) {
    int new_x = pacman->x;
    int new_y = pacman->y;

    // checking if the direction chosen by the user is valid
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

    // updating pacman position and also handling warping
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
        pacman->x = new_x;
        pacman->y = new_y;
    }
    
    // handling power up
    if (maze[pacman->y][pacman->x] == POWERUP) {
        pacman->powered_up = true;
        pacman->powered_up_time_left = 20;
    }
    if (pacman->powered_up_time_left == 0) {
        pacman->powered_up = false;
    }
    else {
        pacman->powered_up_time_left -= 1;
    }
}