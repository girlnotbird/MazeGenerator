#include <vector>
#include <iostream>
#include "2d-maze.h"
#include "2d-node.h"

Maze_2D::Maze_2D(int height, int width)
{
  this->height = {height};
  this->width = {width};
};

/* LEGACY FUNCTION - connect */
// void connect(Node_2D *head, Node_2D *tail, conn_dir dir, Connection* conn)
// {
//   head->setConnection(dir, conn);
//   tail->setConnection((conn_dir)(((int)dir + 2) % 4), conn);
// }

/* LEGACY FUNCTION - Maze_2D::populateNodes */
// void Maze_2D::populateNodes() {
//   for (int nodeIndex = 0; nodeIndex < this->height * this->width; ++nodeIndex)
//   {
//     this->nodes.push_back(Node_2D());
//   }
// };

/* LEGACY FUNCTION - Maze_2D::populateEdges */
// void Maze_2D::populateEdges() {
//   for (int edgeIndex = 0; edgeIndex < (this->height * (this->width -1) + this->width * (this->height -1)); ++edgeIndex) {
//     this->edges.push_back(std::move(*createConnection()));
//   }

//   for (int headIndex = 0; headIndex < this->height * this->width; ++headIndex)
//   {
//     std::pair headCoords {indexToCoords(this->height, this->width, headIndex)};
//     Node_2D *head = &(this->nodes.at(headIndex));

//     std::pair tailCoords {headCoords};
//     Node_2D *tail {};
      
//     // check if node to east exists & create connection
//     if (headCoords.first < this->width-1)
//     {
//       tailCoords.first++;
//       int tailIndex = coordsToIndex(this->height, this->width, tailCoords);
//       tail = &(this->nodes.at(tailIndex));        
//       Connection* newConn = createConnection();
//       this->edges.push_back(std::move(*newConn));
//       tailCoords.first--;
//     }

//     // check if node to south exists & create connection
//     if (headCoords.second < this->height-1)
//     {
//       tailCoords.second++;
//       int tailIndex = coordsToIndex(this->height, this->width, tailCoords);
//       tail = &(this->nodes.at(tailIndex));
//       Connection* newConn = createConnection();
//       this->edges.push_back(std::move(*newConn));
//       tailCoords.second--;
//     }

//     // NOTE: Do not check for north & west connections b/c
//     // those will have already been connected by the node
//     // to the north or west of head.
//   }
// }