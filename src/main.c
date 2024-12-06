#include "../include/maze.h"
#include "../include/render.h"
#include "../include/pacman.h"

int main (int argc, char** argv)
{   
    int width = 1280;
    int height = 720;

    Pacman pacman;
    render("PaC-Man", &width, &height, &pacman);
    return 0;
}