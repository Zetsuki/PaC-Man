#include "../include/utils.h"
#include <stdbool.h>

CellType maze[ROWS][COLS] = 
{
    {CORNER, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, JUNCTION, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, JUNCTION, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, CORNER},
    {MIDDLE_WALL, POWERUP, POINT, POINT, POINT, MIDDLE_WALL, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, MIDDLE_WALL, POINT, POINT, POINT, POINT, MIDDLE_WALL},
    {MIDDLE_WALL, POINT, CORNER, OUTER_WALL, POINT, OUTER_WALL, POINT, OUTER_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, OUTER_WALL, POINT, OUTER_WALL, POINT, OUTER_WALL, CORNER, POINT, MIDDLE_WALL},
    {MIDDLE_WALL, POINT, MIDDLE_WALL, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, MIDDLE_WALL, POINT, MIDDLE_WALL},
    {MIDDLE_WALL, POINT, OUTER_WALL, POINT, OUTER_WALL, OUTER_WALL, POINT, CORNER, OUTER_WALL, EMPTY, EMPTY, OUTER_WALL, CORNER, POINT, OUTER_WALL, OUTER_WALL, POINT, OUTER_WALL, POINT, MIDDLE_WALL},
    {WARP, POINT, POINT, POINT, POINT, POINT, POINT, MIDDLE_WALL, EMPTY, EMPTY, EMPTY, EMPTY, MIDDLE_WALL, POINT, POINT, POINT, POINT, POINT ,POINT, WARP},
    {MIDDLE_WALL, POINT, OUTER_WALL, POINT, OUTER_WALL, OUTER_WALL, POINT, CORNER, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, CORNER, POINT, OUTER_WALL, OUTER_WALL, POINT, OUTER_WALL, POINT, MIDDLE_WALL},
    {MIDDLE_WALL, POINT, MIDDLE_WALL, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, MIDDLE_WALL, POINT, MIDDLE_WALL},
    {MIDDLE_WALL, POINT, CORNER, OUTER_WALL, POINT, OUTER_WALL, POINT, OUTER_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, OUTER_WALL, POINT, OUTER_WALL, POINT, OUTER_WALL, CORNER, POINT, MIDDLE_WALL},
    {MIDDLE_WALL, POINT, POINT, POINT, POINT, MIDDLE_WALL, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, MIDDLE_WALL, POINT, POINT, POINT, POWERUP, MIDDLE_WALL},
    {CORNER, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, JUNCTION, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, JUNCTION, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, MIDDLE_WALL, CORNER}
};

bool is_wall(CellType cell) {
    return cell == MIDDLE_WALL || cell == OUTER_WALL || cell == CORNER || cell == JUNCTION;
}

// checking if the next logical coordinates lead to a wall
bool is_dir_colliding_to_wall(Direction dir, int x, int y) {
    bool res = true;
    switch (dir) {
        case UP:
            if (y > 0 && !is_wall(maze[y - 1][x])) {
                res = false;
            }
            break;
        case DOWN:
            if (y < ROWS - 1 && !is_wall(maze[y + 1][x])) {
                res = false;
            }
            break;
        case LEFT:
            if (x > 0 && !is_wall(maze[y][x - 1])) {
                res = false;
            }
            break;
        case RIGHT:
            if (x < COLS - 1 && !is_wall(maze[y][x + 1])) {
                res = false;
            }
            break;
        default:
            break;
    }

    return res;
}

bool at_crossway(int x, int y) {
    int paths = 0;
    if (!is_dir_colliding_to_wall(UP, x, y)) { paths++;}
    if (!is_dir_colliding_to_wall(DOWN, x, y)) { paths++;}
    if (!is_dir_colliding_to_wall(LEFT, x, y)) { paths++;}
    if (!is_dir_colliding_to_wall(RIGHT, x, y)) { paths++;}

    if(paths >= 3) {
        return true;
    } else {
        return false;
    }  
}