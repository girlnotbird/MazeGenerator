#ifndef _MAZEGEN_H
#define _MAZEGEN_H

#include <map>
#include <vector>
#include <string>
#include <array>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <random>
#include <ctime>

// TODO:   Un-template maze, remove the use of a fixed-size array in favor of a
// TODO:   vector, because currently this needs to know the dimensions of the
// TODO:   map at compile time to generate, and it cannot be passed non-
// TODO:   constant sizes.

// TODO:   Rethink this whole structure. There are memory leaks, there is a re-
// TODO:   cursive algorithm that allocates literal millions of objects in
// TODO:   memory for any maze > 5x5, and crucially, you generate a path by
// TODO:   randomly crawling the grid until a path reaches the end and fulfills
// TODO:   the parameters. That means it can take 2 seconds or 2 years,
// TODO:   depending on the roll, and for most grid sizes it will never
// TODO:   complete. you have to constrain the routing somehow. Break it into
// TODO:   segments that take a fixed amount of time, maybe? And jump back to
// TODO:   previous segment / multiple segments if the route is considered
// TODO:   unsolvable?

///////////////////////////////////////////////////////////////////////////////
// type definitions
///////////////////////////////////////////////////////////////////////////////

enum class direction
{
  N, // 0
  E, // 1
  S, // 2
  W  // 3
};
using Coords = std::pair<int, int>;
using Index = int;
using Display = std::string;

///////////////////////////////////////////////////////////////////////////////
// class definitions
///////////////////////////////////////////////////////////////////////////////

class CustomException : public std::exception
{
private:
  char const *message;

public:
  CustomException(char const *customMessage);
  virtual const char *what() const throw();
};

struct node
{
  typedef std::map<direction, node *> adj_map;
  adj_map adjs;
  std::string name;
  node(std::string _name) : name(_name) {};
  node() : name("__placeholder") {};
};

class graph
{
public:
  typedef std::map<std::string, node *> node_map;
  node_map nodes{};
  node *add_node(const std::string &name);
  void add_edge(const std::string &from, const std::string &to, direction dir);
  void del_edge(const std::string &from, direction dir);
  graph() {};
};

struct pathnode
{
public:
  int depth{};
  node *loc{};
  std::vector<pathnode *> prev{};
  std::vector<pathnode *> children{};
  pathnode() {};
  pathnode(node *_loc) : loc(_loc) {};
  pathnode(node* _loc, pathnode* _prev);
};

// TODO: Use hash-set instead of vector for set of pathnodes?
struct pathnodetree
{
  std::vector<pathnode *> nodes {};
  pathnode* head;
  pathnodetree() : head(nullptr) {};
};

template <int H, int W>
class maze
{
public:
  typedef std::array<node, H * W> grid_2d;
  typedef std::vector<pathnode*> path;

  int height{H};
  int width{W};
  std::string name{};
  bool found_solution{};

  graph layout{};
  grid_2d grid{}; // 2d grid, index can be queried for coords
  pathnodetree *pathtree{};
  pathnode* route{};

  maze(std::string _name) : name(_name) {};
  void init_grid();
  void init_layout();

  void build_path_tree();
  void find_solution();
  void generate_path(pathnode* route);
  void validate_path();

  void render();

private:
  void recursive_build_tree(pathnode * parent);

};

///////////////////////////////////////////////////////////////////////////////
// helper function definitions
///////////////////////////////////////////////////////////////////////////////

Coords getCoordsFromIndex(int h, int w, Index index);
Index getIndexFromCoords(int h, int w, Coords coords);

bool coordsAreOrthogonallyAdjacent(Coords *coords_a, Coords *coords_b);
bool coordsAreSame(Coords *coords_a, Coords *coords_b);

bool isOnNorthWall(Coords *coords, int height, int width);
bool isOnEastWall(Coords *coords, int height, int width);
bool isOnSouthWall(Coords *coords, int height, int width);
bool isOnWestWall(Coords *coords, int height, int width);
bool isOnAnyWall(Coords *coords, int height, int width);

direction getComplementDir(direction dir);
direction onWhichWall(Coords *coords, int height, int width);
direction inWhichDir(node *coords_a, node *coords_b);

void draw_corner(Display *display);
void draw_horizontal_wall(Display *display);
void draw_vertical_wall(Display *display);
void draw_vertical_gap(Display *display);
void draw_path(Display *display);
void draw_N_terminal(Display *display);
void draw_E_terminal(Display *display);
void draw_S_terminal(Display *display);
void draw_W_terminal(Display *display);
void ready_next_row(Display *display);

int randint(int a, int b);

template <int H, int W>
auto create_maze(std::string name)
{
  maze<H, W> *my_maze = new maze<H, W>(name);
  pathnodetree* mazetree;
  mazetree = new pathnodetree();
  my_maze->pathtree = mazetree;
  my_maze->init_grid();
  my_maze->init_layout();
  my_maze->find_solution();
  my_maze->generate_path(my_maze->route);
  my_maze->validate_path();

  return my_maze;
}

///////////////////////////////////////////////////////////////////////////////
// class member function templates
///////////////////////////////////////////////////////////////////////////////

template <int H, int W>
void maze<H, W>::init_layout()
{
  // connect all nodes
  for (int from_index = 0; from_index < H * W; ++from_index)
  {
    Coords from_coords = getCoordsFromIndex(H, W, from_index);
    std::string from_name = std::to_string(from_index);
    node *from = this->layout.nodes.find(from_name)->second;
    Coords to_coords{from_coords};

    // check if node to east exists & create bidirectional edge
    if (from_coords.first < W - 1)
    {
      to_coords.first++;
      int to_index = getIndexFromCoords(H, W, to_coords);
      std::string to_name = std::to_string(to_index);
      this->layout.add_edge(from_name, to_name, direction::E);
      this->layout.add_edge(to_name, from_name, direction::W);
      to_coords.first--;
    }

    // check if node to south exists & create bidirectional edge
    if (from_coords.second < H - 1)
    {
      to_coords.second++;
      int to_index = getIndexFromCoords(H, W, to_coords);
      std::string to_name = std::to_string(to_index);
      this->layout.add_edge(from_name, to_name, direction::S);
      this->layout.add_edge(to_name, from_name, direction::N);
      to_coords.second--;
    }
  }

  // seed random number generator for new maze
  std::srand(time(NULL));

  Coords start_coords = Coords(-1, -1);
  Coords end_coords = Coords(-1, -1);
  this->layout.add_node("start");
  this->layout.add_node("end");

  // pick start
  while (!isOnAnyWall(&start_coords, this->height, this->width))
  {
    start_coords.first = randint(0, this->width - 1);
    start_coords.second = randint(0, this->height - 1);
  }
  int start_index = getIndexFromCoords(this->height, this->width, start_coords);
  direction start_wall = onWhichWall(&start_coords, this->height, this->width);
  this->layout.add_edge("start", std::to_string(start_index), getComplementDir(start_wall));
  this->layout.add_edge(std::to_string(start_index), "start", start_wall);

  // pick end
  while (!isOnAnyWall(&end_coords, this->height, this->width) || coordsAreOrthogonallyAdjacent(&start_coords, &end_coords) || coordsAreSame(&start_coords, &end_coords))
  {
    end_coords.first = randint(0, this->width - 1);
    end_coords.second = randint(0, this->height - 1);
  }
  int end_index = getIndexFromCoords(this->height, this->width, end_coords);
  direction end_wall = onWhichWall(&end_coords, this->height, this->width);
  this->layout.add_edge("end", std::to_string(end_index), getComplementDir(end_wall));
  this->layout.add_edge(std::to_string(end_index), "end", end_wall);
}

template <int H, int W>
void maze<H, W>::init_grid()
{
  // create all nodes & populate grid
  for (int index = 0; index < H * W; index++)
  {
    auto node_name = std::to_string(index);
    node *n = this->layout.add_node(node_name);
    this->grid[index] = std::move(*n);
    this->layout.nodes[node_name] = &(this->grid[index]);
  }
}

template <int H, int W>
void maze<H, W>::render()
{
  Display display{""};

  // render all cells except bottom wall
  for (int y = 0; y < this->height; y++)
  {

    // render top horizontal bounding walls
    for (int x = 0; x < this->width; x++)
    {
      bool is_adjacent_to_terminus = false;
      Display which_terminus{};
      direction terminus_direction = direction::N;
      Coords coords = Coords(x, y);
      int index = getIndexFromCoords(this->height, this->width, coords);
      Display tile_name = std::to_string(index);
      node *tile = &(this->grid[index]);

      if (isOnAnyWall(&coords, this->height, this->width))
      {
        direction dir_of_adjacent_wall = onWhichWall(&coords, this->height, this->width);
        auto a_terminus = tile->adjs.find(dir_of_adjacent_wall);
        if (a_terminus != tile->adjs.end())
        {
          is_adjacent_to_terminus = true;
          which_terminus = a_terminus->second->name;
          terminus_direction = dir_of_adjacent_wall;
        }
      }

      // one tile
      draw_corner(&display);

      // two tiles
      if (is_adjacent_to_terminus && terminus_direction == direction::N)
      {
        if (which_terminus == "start")
        {
          draw_S_terminal(&display);
        }
        if (which_terminus == "end")
        {
          draw_N_terminal(&display);
        }
      }
      else if ((tile->adjs.find(direction::N) == tile->adjs.end()))
      {
        draw_horizontal_wall(&display);
      }
      else
      {
        draw_path(&display);
      }
    }

    // one tile
    draw_corner(&display);

    // terminate line
    ready_next_row(&display);

    // render vertical walls
    for (int x = 0; x < this->width; x++)
    {
      bool is_adjacent_to_terminus = false;
      Display which_terminus{};
      direction terminus_direction = direction::N;
      Coords coords = Coords(x, y);
      int index = getIndexFromCoords(this->height, this->width, coords);
      Display tile_name = std::to_string(index);
      node *tile = &(this->grid[index]);

      if (isOnAnyWall(&coords, this->height, this->width))
      {
        direction dir_of_adjacent_wall = onWhichWall(&coords, this->height, this->width);
        auto a_terminus = tile->adjs.find(dir_of_adjacent_wall);
        if (a_terminus != tile->adjs.end())
        {
          is_adjacent_to_terminus = true;
          which_terminus = a_terminus->second->name;
          terminus_direction = dir_of_adjacent_wall;
        }
      }

      // one tile
      if (is_adjacent_to_terminus && terminus_direction == direction::W)
      {
        if (which_terminus == "end")
        {
          draw_W_terminal(&display);
        }
        if (which_terminus == "start")
        {
          draw_E_terminal(&display);
        }
      }
      else if ((tile->adjs.find(direction::W) == tile->adjs.end()))
      {
        draw_vertical_wall(&display);
      }
      else
      {
        draw_vertical_gap(&display);
      }

      // two tiles
      draw_path(&display);
    }

    // one tile
    int x = this->width - 1;

    bool is_adjacent_to_terminus = false;
    Display which_terminus{};
    direction terminus_direction = direction::N;
    Coords coords = Coords(x, y);
    int index = getIndexFromCoords(this->height, this->width, coords);
    Display tile_name = std::to_string(index);
    node *tile = &(this->grid[index]);

    if (isOnAnyWall(&coords, this->height, this->width))
    {
      direction dir_of_adjacent_wall = onWhichWall(&coords, this->height, this->width);
      auto a_terminus = tile->adjs.find(dir_of_adjacent_wall);
      if (a_terminus != tile->adjs.end())
      {
        is_adjacent_to_terminus = true;
        which_terminus = a_terminus->second->name;
        terminus_direction = dir_of_adjacent_wall;
      }
    }

    if (is_adjacent_to_terminus && terminus_direction == direction::E)
    {
      if (which_terminus == "start")
      {
        draw_W_terminal(&display);
      }
      if (which_terminus == "end")
      {
        draw_E_terminal(&display);
      }
    }
    else if ((tile->adjs.find(direction::E) == tile->adjs.end()))
    {
      draw_vertical_wall(&display);
    }
    else
    {
      draw_vertical_gap(&display);
    }

    // terminate line
    ready_next_row(&display);
  }

  // render bottom wall
  for (int x = 0; x < this->width; x++)
  {
    int y = this->height - 1;

    bool is_adjacent_to_terminus = false;
    Display which_terminus{};
    direction terminus_direction = direction::N;
    Coords coords = Coords(x, y);
    int index = getIndexFromCoords(this->height, this->width, coords);
    Display tile_name = std::to_string(index);
    node *tile = &(this->grid[index]);

    if (isOnAnyWall(&coords, this->height, this->width))
    {
      direction dir_of_adjacent_wall = onWhichWall(&coords, this->height, this->width);
      auto a_terminus = tile->adjs.find(dir_of_adjacent_wall);
      if (a_terminus != tile->adjs.end())
      {
        is_adjacent_to_terminus = true;
        which_terminus = a_terminus->second->name;
        terminus_direction = dir_of_adjacent_wall;
      }
    }

    // one tile
    draw_corner(&display);

    // two tiles
    if (is_adjacent_to_terminus && terminus_direction == direction::S)
    {
      if (which_terminus == "start")
      {
        draw_N_terminal(&display);
      }
      if (which_terminus == "end")
      {
        draw_S_terminal(&display);
      }
    }
    else if ((tile->adjs.find(direction::S) == tile->adjs.end()))
    {
      draw_horizontal_wall(&display);
    }
    else
    {
      draw_path(&display);
    }
  }

  // one tile
  draw_corner(&display);

  // terminate maze
  ready_next_row(&display);

  std::cout << display;
}

template <int H, int W>
void maze<H, W>::build_path_tree()
{
  node *start = this->layout.nodes["start"];
  pathnode *pathtree_head;
  pathtree_head = new pathnode(start);
  pathtree_head->depth = 0;

  if (this->pathtree->nodes.size() > 0)
  {
    for (int index = this->pathtree->nodes.size()-1; index >= 0; --index)
    {
      delete this->pathtree->nodes[index];
      this->pathtree->nodes.erase(this->pathtree->nodes.begin() + index);
    }
  }

  this->pathtree->head = pathtree_head;
  this->pathtree->nodes.push_back(pathtree_head);

  recursive_build_tree(pathtree_head);
};

template <int H, int W>
void maze<H, W>::recursive_build_tree(pathnode * parent)
{
  int starting_dir = randint(0, 4);
  for (int check_dir = 0; check_dir < 4; check_dir++)
  {
    direction actual_dir = (direction)((check_dir + starting_dir) % 4);
    if (parent->loc->adjs.find(actual_dir) == parent->loc->adjs.end()) {
      continue;
    }
    node* adj = parent->loc->adjs[actual_dir];
    pathnode *possible_path;
    possible_path = new pathnode(adj, parent);

    auto is_valid_path = [] (pathnode * candidate)
    {
      for (int hist_index = 0; hist_index < candidate->prev.size(); ++hist_index)
      {
        pathnode *from_hist;
        from_hist = candidate->prev[hist_index];
        if (from_hist->loc->name == candidate->loc->name)
        {
          return false;
        }
      }
      return true;
    };
    
    if (is_valid_path(possible_path))
    {
      parent->children.push_back(possible_path);
      this->pathtree->nodes.push_back(possible_path);

      bool isEnd = possible_path->loc->name=="end" ? true : false;
      bool isLongEnough = possible_path->depth > this->grid.size() * .50 ? true : false;
      bool isNotTooLong = possible_path->depth < this->grid.size() * .90 ? true : false;

      if (isEnd && isLongEnough && isNotTooLong)
      {
        this->found_solution = true;
        this->route = possible_path;
      }
      if (this->found_solution != true) {
        recursive_build_tree(possible_path);
      }
    }
    else
    {
      delete possible_path;
    }
  }
};

template <int H, int W>
void maze<H, W>::generate_path(pathnode* path)
{
  auto route = path->prev;
  // exclude start node, start with first node in maze
  for (int index = 1; index < route.size(); index++) {
    node* thisNode = route[index]->loc;
    node* prevNode = route[index-1]->loc;
    node* nextNode = index+1 < route.size() ? route[index+1]->loc : path->loc;
    direction prevRouteDir = inWhichDir(thisNode, prevNode);
    direction routeDir = inWhichDir(thisNode, nextNode);
    for (int dir = 0; dir < 4; dir++)
    {
      direction removeDir = (direction)dir;
      if (thisNode->adjs.find(removeDir) != thisNode->adjs.end() && removeDir != routeDir && removeDir != prevRouteDir)
      {
        node* nodeToDisconnect = thisNode->adjs[removeDir];
        direction complementaryDir = getComplementDir(removeDir);
        if (nodeToDisconnect != nullptr && nodeToDisconnect->adjs.find(complementaryDir)->second == thisNode)
        {
          nodeToDisconnect->adjs.erase(complementaryDir);
        }
        thisNode->adjs.erase(removeDir);
      }
    }
  }
}

template <int H, int W>
void maze<H, W>::validate_path()
{
  // this->build_path_tree();
  // this->find_solution();
  // if (sols.size() > 1) {
  //   throw CustomException("Invalid path: >1 solutions.");
  // }
  // else if (sols.size() < 1) {
  //   throw CustomException("Invalid path: 0 solutions.");
  // }
}

template <int H, int W>
void maze<H, W>::find_solution()
{
  this->build_path_tree();
}

#endif // _MAZEGEN_H