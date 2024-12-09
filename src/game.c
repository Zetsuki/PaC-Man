#include "../include/game.h"
#include "../include/maze.h"
#include "../include/render.h"

void init_game(GameState* gamestate, unsigned int* width, unsigned int* height) {
    gamestate->pacman = (Pacman*)malloc(sizeof(Pacman));
    if (!gamestate->pacman) {
        printf("Failed to allocate memory for Pacman\n");
        return;
    }
    initialize_pacman(gamestate->pacman);

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
    render(gamestate->render, gamestate->pacman);
    }
    cleanup(gamestate->render->window, gamestate->render->renderer, gamestate->pacman);
}