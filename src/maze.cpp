#include "maze.hpp"
#include "CustomException.hpp"
#include <bitset>
#include <map>

MazeTileMap::MazeTileMap(const int width, const int height)
{
    this->width_ = width;
    this->height_ = height;
    for (auto y = 0; y < this->height_; ++y)
    {
        for (auto x = 0; x < this->width_; ++x)
        {
            MazeTile tile{x, y};
            setTile(x, y, std::move(tile));
        }
    }
}

MazeTile MazeTileMap::getTile(const int x, const int y)
{
    auto coords = MazeCoords{x, y};
    if (this->tiles.find(coords) != this->tiles.end())
    {
        return this->tiles.at(coords);
    }
    throw CustomException("MazeTileMap::getTile: Tile does not exist");
};
MazeTile MazeTileMap::getTile(const MazeCoords& coords)
{
    if (this->tiles.find(coords) != this->tiles.end())
    {
        return this->tiles.at(coords);
    }
    throw CustomException("MazeTileMap::getTile: Tile does not exist");
};
MazeTile MazeTileMap::operator[](const MazeCoords& coords) {return getTile(coords);}

void MazeTileMap::setTile(const int x, const int y, MazeTile&& tile)
{
    auto coords = MazeCoords{x, y};
    this->tiles.insert({coords, tile});
};

auto AsciiMaze::chooseNorthWallDrawFunction(const MazeTile& tile)
{
    bool const northPathIsOpen = (tile.adjFlags & std::bitset<4>(MazeTile::direction::North)) ==
        std::bitset<4>(MazeTile::direction::North);
    if (northPathIsOpen) { return AsciiMaze::drawEmptyHorizontalWall; }
    else { return AsciiMaze::drawHorizontalWall; }
}

auto AsciiMaze::chooseEastWallDrawFunction(const MazeTile& tile)
{
    bool const westPathIsOpen = (tile.adjFlags & std::bitset<4>(MazeTile::direction::East)) ==
        std::bitset<4>(MazeTile::direction::East);
    if (westPathIsOpen) { return AsciiMaze::drawEmptyVerticalWall; }
    else { return AsciiMaze::drawVerticalWall; }
}
auto AsciiMaze::chooseSouthWallDrawFunction(const MazeTile& tile)
{
    bool const westPathIsOpen = (tile.adjFlags & std::bitset<4>(MazeTile::direction::South)) ==
        std::bitset<4>(MazeTile::direction::South);
    if (westPathIsOpen) { return AsciiMaze::drawEmptyHorizontalWall; }
    else { return AsciiMaze::drawHorizontalWall; }
}
auto AsciiMaze::chooseWestWallDrawFunction(const MazeTile& tile)
{
    bool const westPathIsOpen = (tile.adjFlags & std::bitset<4>(MazeTile::direction::West)) ==
        std::bitset<4>(MazeTile::direction::West);
    if (westPathIsOpen) { return AsciiMaze::drawEmptyVerticalWall; }
    else { return AsciiMaze::drawVerticalWall; }
}

void AsciiMaze::renderRowTopBorder(const int row, std::string* output)
{
    for (auto x = 0; x < this->tilemap.width_; ++x)
    {
        AsciiMaze::drawCorner(output);
        auto const tile = this->tilemap.getTile(x, row);
        AsciiMaze::chooseNorthWallDrawFunction(tile)(output);
    }
    AsciiMaze::drawCorner(output);
    AsciiMaze::drawLineReturn(output);
};

void AsciiMaze::renderRowBodyAndSides(const int row, std::string* output)
{
    for (auto x = 0; x < this->tilemap.width_; ++x)
    {
        auto const tile = this->tilemap.getTile(x, row);
        AsciiMaze::chooseWestWallDrawFunction(tile)(output);
        AsciiMaze::drawEmptyCellBody(output);
    }
    auto const tile = this->tilemap.getTile(static_cast<int>(this->tilemap.width_ - 1), row);
    AsciiMaze::chooseEastWallDrawFunction(tile)(output);
    AsciiMaze::drawLineReturn(output);
};

void AsciiMaze::renderRowBottomBorder(const int row, std::string* output)
{
    for (auto x = 0; x < this->tilemap.width_; ++x)
    {
        AsciiMaze::drawCorner(output);
        auto const tile = this->tilemap.getTile(x, row);
        AsciiMaze::chooseSouthWallDrawFunction(tile)(output);
    }
    AsciiMaze::drawCorner(output);
    AsciiMaze::drawLineReturn(output);
};

std::string AsciiMaze::createRenderFrame()
{
    std::string output{};
    for (auto y = 0; y < this->tilemap.height_; ++y)
    {
        AsciiMaze::renderRowTopBorder(y, &output);
        AsciiMaze::renderRowBodyAndSides(y, &output);
    }
    const int y = static_cast<int>(this->tilemap.height_-1);
    AsciiMaze::renderRowBottomBorder(y, &output);

    return output;
}
