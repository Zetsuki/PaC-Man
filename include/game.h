#ifndef GAME_H
#define GAME_H

#include "../include/pacman.h"
#include "../include/render.h"
#include "../include/ghost.h"
#include <stdbool.h>

typedef struct {
    Pacman* pacman;
    Ghost* ghost;
    bool is_game_over;
    RenderState* render; // in charge of rendering the game
} GameState;

void init_game(GameState* gamestate, unsigned int* width, unsigned int* height);

// handle pacman life loss, and ghost being eaten by pac
void pacman_ghost_interaction(GameState* gamestate);

void game_loop(GameState* gamestate);
void free_memory(GameState* gamestate);

#endif