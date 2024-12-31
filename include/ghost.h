#ifndef GHOST_H
#define GHOST_H

#include <stdbool.h>
#include "../include/pacman.h"
#include "../include/utils.h"

typedef struct {
    char* name;
    int x, y; 
    Direction dir; // current direction
    bool back_to_spawn;
    unsigned int memory; // the number of seconds the ghost can chase
    unsigned int current_memory; // the number of seconds the ghost can try to chase out of sight before idling
} Ghost;

void initialize_ghost(Ghost* ghost, char* name);
void update_ghost(Ghost* ghost, Pacman* pacman);

#endif