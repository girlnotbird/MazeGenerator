#ifndef MAZEBUILDER_HPP
#define MAZEBUILDER_HPP

#include "maze.hpp"

bool isOnNorthWall(MazeCoords& coords, AsciiMaze& maze);
bool isOnEastWall(MazeCoords& coords, AsciiMaze& maze);
bool isOnSouthWall(MazeCoords& coords, AsciiMaze& maze);
bool isOnWestWall(MazeCoords& coords, AsciiMaze& maze);
bool isOnAnyWall(MazeCoords& coords, AsciiMaze& maze);
int availableDirections(MazeCoords& coords, AsciiMaze& maze);

MazeTile::direction getComplementDir(MazeTile::direction dir);

class MazeBuilder
{
    public:
    MazeBuilder() = default;

    static void resetMaze(AsciiMaze& maze);
    static void wilsonAlgorithm(AsciiMaze& maze);
};

#endif //MAZEBUILDER_HPP
