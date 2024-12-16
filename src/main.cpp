#include "jm_mazegen.h"

int main()
{
  std::string name = "A-maze-ing";
  auto my_maze = create_maze<8, 8>(name);
  my_maze->render();
  return 0;
}