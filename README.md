# MazeGenerator

MazeGenerator is a c++ program that generates mazes of a given height/width.

## Installation

The code is not bundled into a library with a clean API. To use, clone the repository and build with CMake. Once you've built it, you can run it from the command line by navigating to the root directory and running the following command:

```bash
./build/Mazegen
```
## Usage

```c++
#include "maze.hpp"
#include "mazebuilder.hpp"
#include "renderer.hpp"

// Pass an output stream to the renderer
CommandLineRenderer renderer{&std::cout};

// Generate an empty maze object with a given width & height
AsciiMaze myMaze{15, 15};

// Pass it through a maze Generation algorithm in the MazeBuilder class
MazeBuilder::wilsonAlgorithm(myMaze);

// Render the maze to ascii in the terminal
renderer.render(myMaze.createRenderFrame());
```

## License

MIT
