#ifndef PACMAN_H
#define PACMAN_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
} Direction;

typedef struct {
    int x, y; 
    Direction dir; 
    Direction next_dir;
    bool powered_up;
    unsigned int powered_up_time_left;
} Pacman;

void initialize_pacman(Pacman* pacman);
void handle_pacman_event(SDL_Event* event, Pacman* pacman);
void update_pacman(Pacman* pacman);

#endif