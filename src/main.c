#include "../include/maze.h"
#include "../include/render.h"
#include "../include/pacman.h"
#include "../include/game.h"

int main (int argc, char** argv)
{   
    GameState* gamestate = (GameState*)malloc(sizeof(GameState));
    if (!gamestate) {
        printf("Failed to allocate memory for GameState\n");
        return -1; 
    }
    unsigned int width = 1280;
    unsigned int height = 720;
    init_game(gamestate, &width, &height);
    game_loop(gamestate);
    
    free(gamestate->render);
    free(gamestate->pacman);
    free(gamestate);

    return 0;
}