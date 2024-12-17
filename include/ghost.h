#ifndef GHOST_H
#define GHOST_H

#include <stdbool.h>
#include "../include/utils.h"

typedef struct {
    char* name;
    int x, y; 
    Direction dir; // current direction
    bool back_to_spawn;
} Ghost;

void initialize_ghost(Ghost* ghost, char* name);
void update_ghost(Ghost* ghost);

#endif