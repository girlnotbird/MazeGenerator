#include <iostream>
#include "maze.hpp"
#include "mazebuilder.hpp"
#include "renderer.hpp"

int main()
{
    std::srand(std::clock());
    CommandLineRenderer renderer{&std::cout};

    AsciiMaze myMaze{100, 100};
    MazeBuilder::wilsonAlgorithm(myMaze);
    renderer.render(myMaze.createRenderFrame());

    return 0;
}
