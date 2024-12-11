#include "../include/game.h"
#include "../include/config.h"

int main (int argc, char** argv)
{   
    GameState* gamestate = (GameState*)malloc(sizeof(GameState));
    if (!gamestate) {
        printf("Failed to allocate memory for GameState\n");
        return -1; 
    }
    unsigned int width = WINDOW_WIDTH;
    unsigned int height = WINDOW_HEIGHT;
    init_game(gamestate, &width, &height);
    game_loop(gamestate);
    free_memory(gamestate);

    return 0;
}