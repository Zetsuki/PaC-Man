#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

#define ROWS 11
#define COLS 20

#define TILE_WIDTH 32
#define TILE_HEIGHT 32

#define PACMAN_X 10
#define PACMAN_Y 3

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

typedef enum {
    EMPTY, 
    MIDDLE_WALL,
    OUTER_WALL,
    JUNCTION,
    CORNER, 
    WARP,
    POINT,
    POWERUP
} CellType;

extern CellType maze[ROWS][COLS];

bool is_wall(CellType cell);

#endif