#include "../include/maze.h"
#include "../include/render.h"

int main (int argc, char** argv)
{   
    int width = 1280;
    int height = 720;
    render("PaC-Man", &width, &height);
    return 0;
}