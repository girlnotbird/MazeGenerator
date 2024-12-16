#include <map>
#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <random>

#include "jm_mazegen.h"

///////////////////////////////////////////////////////////////////////////////
// class member function definitions
///////////////////////////////////////////////////////////////////////////////

CustomException::CustomException(char const *customMessage)
{
  this->message = customMessage;
};
const char *CustomException::what() const throw()
{
  return this->message;
};

node *graph::add_node(const std::string &name)
{
  node_map::iterator itr = nodes.find(name);
  if (itr == nodes.end())
  {
    node *n;
    n = new node(name);
    nodes[name] = n;
    return n;
  }
  std::cerr << "Node already exists!";
  throw CustomException("Node already exists!");
}
void graph::add_edge(const std::string &from, const std::string &to, direction dir)
{
  node *f = nodes.find(from)->second;
  node *t = nodes.find(to)->second;
  std::pair<direction, node *> conn = std::make_pair(dir, t);
  f->adjs.insert(conn);
}
void graph::del_edge(const std::string &from, direction dir)
{
  node *f = nodes.find(from)->second;
  f->adjs.erase(dir);
};

///////////////////////////////////////////////////////////////////////////////
// helper function implementations
///////////////////////////////////////////////////////////////////////////////

Coords getCoordsFromIndex(int h, int w, Index index)
{
  return std::pair(index % w, (index - (index % w)) / w);
};
Index getIndexFromCoords(int h, int w, Coords coords)
{
  return coords.second * w + coords.first;
};

bool coordsAreOrthogonallyAdjacent(Coords *coords_a, Coords *coords_b)
{
  // returns true if only one of either x_diff or y_diff are equal to zero
  return (coords_a->first == coords_b->first) != (coords_a->second == coords_b->second);
}
bool coordsAreSame(Coords *coords_a, Coords *coords_b)
{
  // returns true if x_diff and y_diff are both zero
  return (coords_a->first == coords_b->first) && (coords_a->second == coords_b->second);
}

bool isOnNorthWall(Coords *coords, int height, int width)
{
  return coords->second == 0;
}
bool isOnEastWall(Coords *coords, int height, int width)
{
  return coords->first == width - 1;
}
bool isOnSouthWall(Coords *coords, int height, int width)
{
  return coords->second == height - 1;
}
bool isOnWestWall(Coords *coords, int height, int width)
{
  return coords->first == 0;
}
bool isOnAnyWall(Coords *coords, int height, int width)
{
  return (
      isOnNorthWall(coords, height, width) ||
      isOnEastWall(coords, height, width) ||
      isOnSouthWall(coords, height, width) ||
      isOnWestWall(coords, height, width));
}

direction getComplementDir(direction dir)
{
  return (direction)(((int)dir + 2) % 4);
};
direction onWhichWall(Coords *coords, int height, int width)
{
  if (isOnNorthWall(coords, height, width))
  {
    return direction::N;
  }
  else if (isOnEastWall(coords, height, width))
  {
    return direction::E;
  }
  else if (isOnSouthWall(coords, height, width))
  {
    return direction::S;
  }
  else if (isOnWestWall(coords, height, width))
  {
    return direction::W;
  }
  else
  {
    throw CustomException("Coords are not on any wall.");
  }
}
direction inWhichDir(node* node_a, node* node_b)
{
  for (int index = 0; index < 4; index++)
  {
    direction dir = (direction)index;
    if (node_a->adjs[dir] == node_b) {
      return dir;
    }
  }
  throw CustomException("node_b is not adjacent to node_a");
}

void draw_corner(Display *display)
{
  display->append("+");
};
void draw_horizontal_wall(Display *display)
{
  display->append("---");
};
void draw_vertical_wall(Display *display)
{
  display->append("|");
};
void draw_vertical_gap(Display *display)
{
  display->append(" "); // one space to replace vertical wall
};
void draw_path(Display *display)
{
  display->append("   "); // two spaces for inside tile / to replace horizontal wall
};
void ready_next_row(Display *display)
{
  display->append("\n");
};
void draw_N_terminal(Display *display)
{
  display->append(" ^ ");
};
void draw_E_terminal(Display *display)
{
  display->append(">");
};
void draw_S_terminal(Display *display)
{
  display->append(" v ");
};
void draw_W_terminal(Display *display)
{
  display->append("<");
};

int randint(int a, int b)
{
  int range = std::abs(b - a) + 1;
  int random = std::rand();
  int return_value = (random % range) + a;
  return return_value;
}

pathnode::pathnode(node *_loc, pathnode *_prev)
{
  this->loc = _loc;
  this->depth = _prev->depth + 1;

  for (int hist_index = 0; hist_index < _prev->prev.size(); ++hist_index)
  {
    pathnode *from_hist;
    from_hist = _prev->prev[hist_index];
    this->prev.push_back(from_hist);
  }
  this->prev.push_back(_prev);
}