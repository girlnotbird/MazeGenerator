#ifndef _2D_MAZE_H
#define _2D_MAZE_H

#include <map>

#include "./2d-node.h"

class Maze_2D {
  public:
    int height;
    int width;
    std::map<unsigned int, Node_2D> nodes;
    std::map<unsigned int, std::string> nodeAdjacencies;
    Maze_2D(int height, int width);
  private:
    void createNodes();
    void buildAdjacencyList();
};

#endif // _2D_MAZE_H