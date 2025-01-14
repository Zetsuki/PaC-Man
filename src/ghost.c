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

// shuffle a directions array
void shuffle(Direction* directions, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Direction temp = directions[i];
        directions[i] = directions[j];
        directions[j] = temp;
    }
}

Direction get_idling_direction(Ghost* ghost) {
    Direction valid_directions[4];
    int count = 0;

    if (!is_dir_colliding_to_wall(UP, ghost->x, ghost->y)) {
        valid_directions[count++] = UP;
    }
    if (!is_dir_colliding_to_wall(DOWN, ghost->x, ghost->y)) {
        valid_directions[count++] = DOWN;
    }
    if (!is_dir_colliding_to_wall(LEFT, ghost->x, ghost->y)) {
        valid_directions[count++] = LEFT;
    }
    if (!is_dir_colliding_to_wall(RIGHT, ghost->x, ghost->y)) {
        valid_directions[count++] = RIGHT;
    }

    shuffle(valid_directions, count);

    return valid_directions[0];
}

void update_ghost(Ghost* ghost, Pacman* pacman) {
    // idling/roaming
    if (ghost->current_memory == 0) {
        // time to change direction, ghost is against a wall or at a crossway (50% chance for the ghost to attempt a direction change at crossway) 
        if ((is_dir_colliding_to_wall(ghost->dir, ghost->x, ghost->y)) || (at_crossway(ghost->x, ghost->y) && (rand() % 2 == 0))) {
            ghost->dir = get_idling_direction(ghost);
        } 

        switch(ghost->dir) {
            case UP:
                ghost->y -= 1;
                break;
            case DOWN:
                ghost->y += 1;
                break;
            case LEFT:
                if (ghost->x > 0 && maze[ghost->y][ghost->x - 1] != WARP) {
                    ghost->x -= 1; 
                } else if (maze[ghost->y][ghost->x - 1] == WARP) {
                    ghost->x = COLS - 1;
                }       
                break;
            case RIGHT:
                if (ghost->x < COLS - 1 && maze[ghost->y][ghost->x + 1] != WARP) {
                    ghost->x += 1;
                } else if (maze[ghost->y][ghost->x + 1] == WARP) {
                    ghost->x = 0; 
                }
                break;  
            default:
                break;
        }
    }
    // chasing
    else {
    }
}