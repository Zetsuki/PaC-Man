#include <stdio.h>
#include "../include/ghost.h"

void initialize_ghost(Ghost* ghost) {
    ghost->x = PACMAN_X;
    ghost->y = PACMAN_Y;
    ghost->dir = NONE;
    ghost->is_vulnerable = false;
}