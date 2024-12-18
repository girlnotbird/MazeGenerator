#include <iostream>
#include "maze.hpp"
#include "mazebuilder.hpp"
#include "renderer.hpp"

int main()
{
    std::srand(std::clock());
    CommandLineRenderer renderer{&std::cout};

    int width {};
    int height {};

    std::cout << "Let's generate a maze! What do you want its dimensions to be?" << "\n";
    std::cout << "width: ";
    std::cin >> width;
    std::cout << "height: ";
    std::cin >> height;
    std::cout << "\n" << "Alright! Generating Maze..." << "\n\n";

    AsciiMaze myMaze{width, height};
    MazeBuilder::wilsonAlgorithm(myMaze);
    renderer.render(myMaze.createRenderFrame());

    std::cout << "\n" << "Can you solve it?" << "\n\n";

    return 0;
}
