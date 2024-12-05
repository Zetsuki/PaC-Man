#ifndef maze_H
#define maze_H

typedef enum {
    EMPTY, 
    WALL, 
    POINT,
    POWERUP
} CellType;

void display_window(const char *title, int width, int height);

#endif