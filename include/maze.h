#ifndef maze_H
#define maze_H

typedef enum {
    EMPTY, 
    MIDDLE_WALL,
    OUTER_WALL,
    JUNCTION,
    CORNER, 
    POINT,
    POWERUP
} CellType;

void display_window(const char *title, int width, int height);

#endif