#ifndef MAZE_HPP
#define MAZE_HPP

#include <bitset>
#include <map>

using MazeCoords = std::pair<int, int>;

struct MazeTile
{
    enum direction
    {
        North = 0b0001,
        East = 0b0010,
        South = 0b0100,
        West = 0b1000
    };

    MazeCoords const position;
    std::bitset<4> adjFlags{0b0000};
    bool connected{false};

    MazeTile(const int x, const int y) : position{x, y} {}
    MazeTile(const int x, const int y, const int flags) : position{x, y}, adjFlags(flags) {}

    ~MazeTile() = default;
    MazeTile(const MazeTile& other) = default;
    MazeTile(MazeTile&& other) noexcept = default;
    MazeTile& operator=(const MazeTile& other);
    MazeTile& operator=(MazeTile&& other) noexcept;
};

class MazeTileMap
{
public:
    size_t width_{0};
    size_t height_{0};
    std::map<MazeCoords, MazeTile> tiles{};
    MazeTile getTile(const MazeCoords& coords);
    MazeTile getTile(int x, int y);
    MazeTile operator[](const MazeCoords& coords);

    MazeTileMap() = default;
    MazeTileMap(int width, int height);
    void setTile(int x, int y, MazeTile&& tile);
};

template <typename Frame>
class Maze
{
public:
    MazeTileMap tilemap;
    int height { static_cast<int>(tilemap.height_)};
    int width { static_cast<int>(tilemap.width_)};

    virtual Frame createRenderFrame() = 0;

    virtual ~Maze() = default;
    Maze(const int width, const int height) : tilemap(MazeTileMap{width, height}) {};
};

class AsciiMaze final : public Maze<std::string>
{
public:
    // MazeTileMap tilemap;
    // int height;
    // int width;

    std::string createRenderFrame() override;

    AsciiMaze(const int width, const int height) : Maze<std::string>{width, height} {};
    ~AsciiMaze() override = default;

    MazeTile getTile(const MazeCoords& coords) { return this->tilemap.getTile(coords); };
    MazeTile getTile(int x, int y) { return this->tilemap.getTile(x, y); };
    MazeTile operator[](const MazeCoords& coords) { return this->tilemap[coords]; };

private:
    static void drawCorner(std::string* output) { output->append("*"); };
    static void drawHorizontalWall(std::string* output) { output->append("---"); };
    static void drawVerticalWall(std::string* output) { output->append("|"); };
    static void drawEmptyVerticalWall(std::string* output) { output->append(" "); };
    static void drawEmptyHorizontalWall(std::string* output) { output->append("   "); };
    static void drawEmptyCellBody(std::string* output) { output->append("   "); };
    static void drawNorthTerminal(std::string* output) { output->append(" ^ "); };
    static void drawEastTerminal(std::string* output) { output->append(">"); };
    static void drawSouthTerminal(std::string* output) { output->append(" v "); };
    static void drawWestTerminal(std::string* output) { output->append("<"); };
    static void drawLineReturn(std::string* output) { output->append("\n"); };

    static auto chooseNorthWallDrawFunction(const MazeTile& tile);
    static auto chooseEastWallDrawFunction(const MazeTile& tile);
    static auto chooseSouthWallDrawFunction(const MazeTile& tile);
    static auto chooseWestWallDrawFunction(const MazeTile& tile);

    void renderRowTopBorder(int row, std::string* output);
    void renderRowBottomBorder(int row, std::string* output);
    void renderRowBodyAndSides(int row, std::string* output);
    void renderFinalLine(int row, std::string* output);
};
#endif //MAZE_HPP
