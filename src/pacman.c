#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../include/pacman.h"
#include "../include/maze.h"
#include "../include/render.h"

bool is_wall(CellType cell) {
    return cell == MIDDLE_WALL || cell == OUTER_WALL || cell == CORNER || cell == JUNCTION;
}

void initialize_pacman(SDL_Renderer* renderer, Pacman* pacman) {
    pacman->x = 10;
    pacman->y = 3;
    pacman->dir = NONE;
    pacman->next_dir = NONE;
    pacman->texture = load_texture(renderer, "assets/pacman.png");
    if (!pacman->texture) {
        printf("Failed to load Pac-Man texture.\n");
    }
}

void handle_pacman_event(SDL_Event* event, Pacman* pacman) {
    if (event->type == SDL_KEYDOWN) {
        Direction new_dir = NONE;
        switch (event->key.keysym.sym) {
            case SDLK_UP:
                new_dir = UP;
                break;
            case SDLK_DOWN:
                new_dir = DOWN;
                break;
            case SDLK_LEFT:
                new_dir = LEFT;
                break;
            case SDLK_RIGHT:
                new_dir = RIGHT;
                break;
        }

        if ((pacman->dir == UP && new_dir == DOWN) ||
            (pacman->dir == DOWN && new_dir == UP) ||
            (pacman->dir == LEFT && new_dir == RIGHT) ||
            (pacman->dir == RIGHT && new_dir == LEFT)) {
            return;
        }

        pacman->next_dir = new_dir;
    }
}

void update_pacman(Pacman* pacman, CellType maze[ROWS][COLS]) {
    int new_x = pacman->x;
    int new_y = pacman->y;

    if (pacman->next_dir != NONE) {
        switch (pacman->next_dir) {
            case UP:
                if (pacman->y > 0 && !is_wall(maze[pacman->y - 1][pacman->x])) {
                    pacman->dir = UP;
                }
                break;
            case DOWN:
                if (pacman->y < ROWS - 1 && !is_wall(maze[pacman->y + 1][pacman->x])) {
                    pacman->dir = DOWN;
                }
                break;
            case LEFT:
                if (pacman->x > 0 && !is_wall(maze[pacman->y][pacman->x - 1])) {
                    pacman->dir = LEFT;
                }
                break;
            case RIGHT:
                if (pacman->x < COLS - 1 && !is_wall(maze[pacman->y][pacman->x + 1])) {
                    pacman->dir = RIGHT;
                }
                break;
            default:
                break;
        }
    }

    switch (pacman->dir) {
        case UP:
            new_y -= 1;
            break;
        case DOWN:
            new_y += 1;
            break;
        case LEFT:
            new_x -= 1;
            break;
        case RIGHT:
            new_x += 1;
            break;
        default:
            break;
    }

    if (new_x >= 0 && new_x < COLS && new_y >= 0 && new_y < ROWS && !is_wall(maze[new_y][new_x])) {

        maze[pacman->y][pacman->x] = EMPTY;

        pacman->x = new_x;
        pacman->y = new_y;
    }
}



void render_pacman(SDL_Renderer* renderer, Pacman* pacman, int* width, int* height) {
    int tile_width = 32;
    int tile_height = 32;

    float scale_x = (float)*width / (COLS * tile_width);
    float scale_y = (float)*height / (ROWS * tile_height);
    float scale = scale_x < scale_y ? scale_x : scale_y;
    scale *= 0.85f;

    int offset_x = (*width - (COLS * tile_width * scale)) / 2;
    int offset_y = (*height - (ROWS * tile_height * scale)) / 2;

    SDL_Rect destRect = {
        offset_x + pacman->x * tile_width * scale,
        offset_y + pacman->y * tile_height * scale,
        tile_width * scale,
        tile_height * scale
    };

    SDL_RenderCopy(renderer, pacman->texture, NULL, &destRect);
}

void cleanup_pacman(Pacman* pacman) {
    SDL_DestroyTexture(pacman->texture);
}
