#ifndef GAME_H
#define GAME_H

#include "../include/pacman.h"
#include "../include/maze.h"
#include "../include/config.h"
#include "../include/render.h"
#include <stdbool.h>

typedef struct {
    Pacman* pacman;
    bool is_game_over;
    RenderState* render;
} GameState;

void init_game(GameState* gamestate, unsigned int* width, unsigned int* height);
void game_loop(GameState* gamestate);

#endif