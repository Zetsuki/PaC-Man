#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

#define ROWS 11
#define COLS 20

#define TILE_WIDTH 32
#define TILE_HEIGHT 32

#define PACMAN_X 10
#define PACMAN_Y 3

#define BLINKY_X 11
#define BLINKY_Y 5

#define PINKY_X 10
#define PINKY_Y 3

#define INKY_X 10
#define INKY_Y 3

#define CLYDE_X 10
#define CLYDE_Y 3

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
} Direction;

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
bool is_dir_colliding_to_wall(Direction dir, int x, int y); // checking if the next move toward the Direction dir leads to a wall
bool at_crossway(int x, int y); // 3 or more directions available at x,y

#endif