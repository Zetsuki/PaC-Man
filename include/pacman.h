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
    SDL_Texture* texture; 
    SDL_Texture* powered_up_texture;
    bool powered_up;
    unsigned int powered_up_time_left;
} Pacman;

void initialize_pacman(Pacman* pacman);
void load_all_pac_textures(SDL_Renderer* renderer, Pacman* pacman);
void handle_pacman_event(SDL_Event* event, Pacman* pacman);
void update_pacman(Pacman* pacman);
void render_pacman(SDL_Renderer* renderer, Pacman* pacman, unsigned int* width, unsigned int* height);
void cleanup_pacman(Pacman* pacman);

#endif