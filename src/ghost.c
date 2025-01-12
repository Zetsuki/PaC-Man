#include <stdio.h>
#include <string.h>
#include "../include/ghost.h"

void initialize_ghost(Ghost* ghost, char* name) {
    ghost->name = name;
    if(strcmp(name,"BLINKY") == 0) {
        ghost->x = BLINKY_X;
        ghost->y = BLINKY_Y;
    } 
    else if(strcmp(name,"PINKY") == 0) {
        ghost->x = PINKY_X;
        ghost->y = PINKY_Y;
    }
    else if(strcmp(name,"INKY") == 0) {
        ghost->x = INKY_X;
        ghost->y = INKY_Y;
    }
    else if(strcmp(name,"CLYDE") == 0) {
        ghost->x = CLYDE_X;
        ghost->y = CLYDE_Y;
    }
    ghost->dir = NONE;
    ghost->back_to_spawn = false;
    ghost->memory = 8;
    ghost->current_memory = 0;
}

void random_move(Ghost* ghost) {
    bool done = false;
    while (!done) {
        ghost->dir = (Direction)(rand() % 4);
        switch (ghost->dir) {
            case UP:
                if (ghost->y > 0 && !is_wall(maze[ghost->y - 1][ghost->x])) {
                    ghost->dir = UP;
                    ghost->y = ghost->y + 1;
                    done = true;
                }
                break;
            case DOWN:
                if (ghost->y < ROWS - 1 && !is_wall(maze[ghost->y + 1][ghost->x])) {
                    ghost->dir = DOWN;
                    ghost->y = ghost->y - 1;
                    done = true;
                }
                break;
            case LEFT:
                if (ghost->x > 0 && !is_wall(maze[ghost->y][ghost->x - 1])) {
                    ghost->dir = LEFT;
                    ghost->x = ghost->x - 1;
                    done = true;
                }
                break;
            case RIGHT:
                if (ghost->x < COLS - 1 && !is_wall(maze[ghost->y][ghost->x + 1])) {
                    ghost->dir = RIGHT;
                    ghost->x = ghost->x + 1;
                    done = true;
                }
                break;
            default:
                break;
        }
    }
}


void update_ghost(Ghost* ghost, Pacman* pacman) {
    // idling/roaming
    if(ghost->current_memory == 0) {
        random_move(ghost);
    }
    // chasing
    else {
        random_move(ghost);
    }
}