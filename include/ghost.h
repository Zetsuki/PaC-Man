#ifndef GHOST_H
#define GHOST_H

#include <stdbool.h>
#include "../include/utils.h"

typedef struct {
    int x, y; 
    Direction dir; // current direction
    bool is_vulnerable;
} Ghost;

void initialize_ghost(Ghost* ghost);

#endif