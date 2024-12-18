#include "../include/mazebuilder.hpp"

#include <iostream>
#include <ostream>

#include "../include/randint.hpp"
#include "CustomException.hpp"
#include <random>

bool isOnNorthWall(const MazeCoords& coords, const AsciiMaze& maze)
{
    return coords.second == 0;
}

bool isOnEastWall(const MazeCoords& coords, const AsciiMaze& maze)
{
    return coords.first == maze.width - 1;
}

bool isOnSouthWall(const MazeCoords& coords, const AsciiMaze& maze)
{
    return coords.second == maze.height - 1;
}

bool isOnWestWall(const MazeCoords& coords, const AsciiMaze& maze)
{
    return coords.first == 0;
}

bool isOnAnyWall(const MazeCoords& coords, const AsciiMaze& maze)
{
    return (
        isOnNorthWall(coords, maze) ||
        isOnEastWall(coords, maze) ||
        isOnSouthWall(coords, maze) ||
        isOnWestWall(coords, maze));
}

MazeTile::direction getComplementDir(const MazeTile::direction dir)
{
    switch (dir)
    {
    case MazeTile::direction::North: return MazeTile::direction::South;
    case MazeTile::direction::East: return MazeTile::direction::West;
    case MazeTile::direction::South: return MazeTile::direction::North;
    case MazeTile::direction::West: return MazeTile::direction::East;
    }
    throw CustomException("Unknown direction");
};

MazeTile::direction adjacentInWhichDirection(const std::pair<MazeCoords, MazeTile*>& firstEntry,
                                             const std::pair<MazeCoords, MazeTile*>& secondEntry)
{
    auto [firstCoords, firstTile] = firstEntry;
    auto [secondCoords, secondTile] = secondEntry;
    if (firstCoords.second - 1 == secondCoords.second) { return MazeTile::direction::North; }
    else if (firstCoords.first + 1 == secondCoords.first) { return MazeTile::direction::East; }
    else if (firstCoords.second + 1 == secondCoords.second) { return MazeTile::direction::South; }
    else if (firstCoords.first - 1 == secondCoords.first) { return MazeTile::direction::West; }
    else { throw CustomException("Tiles not adjacent"); }
}

int availableDirections(const MazeCoords& coords, const AsciiMaze& maze)
{
    int result = 0b0000;
    if (!isOnNorthWall(coords, maze))
        result ^= MazeTile::direction::North;
    if (!isOnEastWall(coords, maze))
        result ^= MazeTile::direction::East;
    if (!isOnSouthWall(coords, maze))
        result ^= MazeTile::direction::South;
    if (!isOnWestWall(coords, maze))
        result ^= MazeTile::direction::West;

    return result;
}

void MazeBuilder::resetMaze(AsciiMaze& maze)
{
    for (int y = 0; y < maze.height; ++y)
    {
        for (int x = 0; x < maze.width; ++x)
        {
            auto const coords = MazeCoords(x, y);
            auto tile = maze[coords];
            tile.adjFlags &= 0b0000;
        }
    }
}

void MazeBuilder::wilsonAlgorithm(AsciiMaze& maze)
{
    // init algo state
    resetMaze(maze);
    std::map<MazeCoords, MazeTile*> connectedTiles{};
    std::map<MazeCoords, MazeTile*> unconnectedTiles{};
    auto loadInTiles = [&]()
    {
        for (auto [coords, tile] : maze.tilemap.tiles)
        {
            MazeTile* tileAddr = &(maze.tilemap.tiles.at(coords));
            unconnectedTiles.insert({coords, tileAddr});
        }
    };
    loadInTiles();
    auto previousTile = unconnectedTiles.begin();
    auto currentTile = unconnectedTiles.begin(); // begin(), a tile*, or end()
    MazeCoords nextCoords = currentTile->first;

    // define algo functions
    auto pickRandomFromMap = [&](std::map<MazeCoords, MazeTile*>& map)
    {
        bool found = false;
        auto item = map.begin();
        while (!found)
        {
            if (map.empty()) { break; }
            std::advance(item, randint(0, static_cast<int>(map.size() - 1)));
            found = true;
        }
        return item;
    };

    // TODO: Make this also change the bit flags to actually connect the edges
    auto loopTerminatedRandomWalk = [&](const std::pair<MazeCoords, MazeTile*>& start,
                                        const std::pair<MazeCoords, MazeTile*>& end)
    {
        if (unconnectedTiles.empty() == true) { return; }

        std::vector<std::pair<MazeCoords, MazeTile*>> tilesInCurrentWalk{};
        tilesInCurrentWalk.push_back(start);
        bool terminated = false;

        auto findNextStep = [&]()
        {
            auto [coords, tile] = *currentTile;
            auto possibleDirections = availableDirections(coords, maze);
            while (nextCoords == coords)
            {
                const int random = randint(0, 3);
                MazeTile::direction chooseDir;
                if (random == 0) { chooseDir = MazeTile::direction::North; }
                else if (random == 1) { chooseDir = MazeTile::direction::East; }
                else if (random == 2) { chooseDir = MazeTile::direction::South; }
                else if (random == 3) { chooseDir = MazeTile::direction::West; }
                else { throw CustomException("random index out of bounds"); }
                const bool isValidDirection = (possibleDirections & chooseDir) == chooseDir;
                if (!isValidDirection)
                {
                    continue;
                }
                if (chooseDir == MazeTile::direction::North)
                {
                    nextCoords = MazeCoords{coords.first, coords.second - 1};
                }
                else if (chooseDir == MazeTile::direction::East)
                {
                    nextCoords = MazeCoords{coords.first + 1, coords.second};
                }
                else if (chooseDir == MazeTile::direction::South)
                {
                    nextCoords = MazeCoords{coords.first, coords.second + 1};
                }
                else // MazeTile::direction::West
                {
                    nextCoords = MazeCoords{coords.first - 1, coords.second};
                }
                if (nextCoords == previousTile->first)
                {
                    // go through the loop again, we don't want to step immediately backwards
                    nextCoords = coords;
                }
            }
        };

        // TODO: Make this also change the bitmasks to connect the edges
        auto executeNextStep = [&]()
        {
            bool isConnected = connectedTiles.find(nextCoords) != connectedTiles.end();
            auto tileItrAtLocation = isConnected ? connectedTiles.find(nextCoords) : unconnectedTiles.find(nextCoords);
            std::pair<MazeCoords, MazeTile*> tileToSearch = std::pair(tileItrAtLocation->first,
                                                                      tileItrAtLocation->second);
            bool haveAlreadySteppedOn = std::find(tilesInCurrentWalk.begin(), tilesInCurrentWalk.end(), tileToSearch) !=
                tilesInCurrentWalk.end();
            bool isEnd = unconnectedTiles.find(nextCoords) != unconnectedTiles.end() && unconnectedTiles.
                find(nextCoords)->first == end.first;

            if (isConnected || isEnd)
            {
                tilesInCurrentWalk.emplace_back(tileToSearch);
                for (auto i = 0; i < tilesInCurrentWalk.size(); ++i)
                {
                    auto firstEntry = tilesInCurrentWalk.at(i);
                    if (i + 1 < tilesInCurrentWalk.size())
                    {
                        auto secondEntry = tilesInCurrentWalk.at(i + 1);
                        MazeTile::direction adjDir = adjacentInWhichDirection(firstEntry, secondEntry);
                        MazeTile::direction complementDir = getComplementDir(adjDir);
                        firstEntry.second->adjFlags ^= adjDir;
                        secondEntry.second->adjFlags ^= complementDir;
                        firstEntry.second->connected = true;
                    }
                    // TODO: Make this also change the bitmasks to connect the edges
                    connectedTiles.insert({std::move(firstEntry)});
                    unconnectedTiles.erase(firstEntry.first);
                }
                tilesInCurrentWalk.erase(tilesInCurrentWalk.begin(), tilesInCurrentWalk.end());
                currentTile = pickRandomFromMap(unconnectedTiles);
                previousTile = currentTile;
                nextCoords = currentTile->first;
                terminated = true;
            }
            else
            {
                bool tilesAdjacentXbutNotY = (tileItrAtLocation->first.first == currentTile->first.first + 1 ||
                        tileItrAtLocation->first.first == currentTile->first.first - 1) && tileItrAtLocation->first.
                    second
                    == currentTile->first.second;
                bool tilesAdjacentYbutNotX = (tileItrAtLocation->first.second == currentTile->first.second + 1 ||
                        tileItrAtLocation->first.second == currentTile->first.second - 1) && tileItrAtLocation->first.
                    first
                    == currentTile->first.first;
                if (!(tilesAdjacentXbutNotY || tilesAdjacentYbutNotX))
                {
                    throw CustomException("Invalid Tile");
                }
                previousTile = currentTile;
                currentTile = tileItrAtLocation;
                nextCoords = currentTile->first;
                if (haveAlreadySteppedOn)
                {
                    tilesInCurrentWalk.erase(
                        std::find(tilesInCurrentWalk.begin(), tilesInCurrentWalk.end(), tileToSearch),
                        tilesInCurrentWalk.end());
                }
                tilesInCurrentWalk.emplace_back(*currentTile);
            }
        };

        while (terminated == false)
        {
            while (currentTile->first == nextCoords)
            {
                findNextStep();
                // TODO: Figure out why this is not correctly setting the next coords
            }
            executeNextStep();
        }
    };

    // algo loop
    while (unconnectedTiles.empty() != true)
    {
        auto start = pickRandomFromMap(unconnectedTiles);
        auto end = connectedTiles.empty() ? pickRandomFromMap(unconnectedTiles) : pickRandomFromMap(connectedTiles);
        currentTile = start;
        nextCoords = currentTile->first;

        loopTerminatedRandomWalk(*start, *end);
    }

    maze.tilemap.tiles.at({0, 0}).adjFlags ^= MazeTile::direction::North;
    maze.tilemap.tiles.at({maze.width - 1, maze.height - 1}).adjFlags ^= MazeTile::direction::South;
}
