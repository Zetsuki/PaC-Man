#ifndef PACMAN_H
#define PACMAN_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "../include/utils.h"

typedef struct {
    int x, y; 
    Direction dir; // current direction
    Direction next_dir; // next direction
    bool powered_up;
    unsigned int powered_up_time_left;
} Pacman;

void initialize_pacman(Pacman* pacman);
void handle_pacman_event(SDL_Event* event, Pacman* pacman); // handling user inputs
void update_pacman(Pacman* pacman); // updating pacman movement, position and power ups

#endif