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
}

void update_ghost(Ghost* ghost) {
    
}