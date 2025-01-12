#include "../include/pacman.h"
#include "../include/render.h"
#include "../include/game.h"

// initialize all the gamestate elements
void init_game(GameState* gamestate, unsigned int* width, unsigned int* height) {
    gamestate->pacman = (Pacman*)malloc(sizeof(Pacman));
    if (!gamestate->pacman) {
        printf("Failed to allocate memory for Pacman\n");
        return;
    }
    initialize_pacman(gamestate->pacman);

    gamestate->ghost = (Ghost*)malloc(sizeof(Ghost));
    if (!gamestate->ghost) {
        printf("Failed to allocate memory for Pacman\n");
        return;
    }
    initialize_ghost(gamestate->ghost, "BLINKY");

    gamestate->render = (RenderState*)malloc(sizeof(RenderState));
    if (!gamestate->render) {
        printf("Failed to allocate memory for RenderState\n");
        free(gamestate->pacman); 
        return;
    }
    gamestate->render->width = width;
    gamestate->render->height = height;
    initialize_render(gamestate->render, gamestate->pacman);    

    gamestate->pacman = gamestate->pacman;
    gamestate->is_game_over = false;
}

// handle pacman life loss, and ghost being eaten by pac
void pacman_ghost_interaction(GameState* gamestate) {

}

// the main loop for the game
void game_loop(GameState* gamestate)  {
    SDL_Event event;
    while (gamestate->is_game_over != true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gamestate->is_game_over = true;
            }
            // window resizing
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                *gamestate->render->width = event.window.data1;
                *gamestate->render->height = event.window.data2;
            }
            handle_pacman_event(&event, gamestate->pacman);
        }
    update_pacman(gamestate->pacman);
    update_ghost(gamestate->ghost, gamestate->pacman);
    render(gamestate->render, gamestate->pacman, gamestate->ghost);
    }
    cleanup(gamestate->render, gamestate->pacman);
}

void free_memory(GameState* gamestate) {
    free(gamestate->render);
    free(gamestate->pacman);
    free(gamestate->ghost);
    free(gamestate);
}