// Project: Maze Problem solving in c++
// Name of an author: Nikolić Dalibor SV13-2023
// Date and time of the last changes: 23.06.2025. 18:47

#ifndef SECTOR_HEADER
#define SECTOR_HEADER

#include <vector>
#include <utility>
#include <unordered_set>
#include "Dynamic.hpp"
#include "CoordHash.hpp"

// Enumeration Direction -> standing for random positions we can choose from pathing or sector position
enum Direction { UP = 1, DOWN = 2, RIGHT = 3, LEFT = 4 };

/* Class Sector -> main goal of making this project to work is to get safe pathing from entrance(U) to exit(I)
   to enable that, we split maze into non-existing sectors with the same dimensions
   from this point we make first sector, by the exit, using random numbers we determine the position of the next sector
   the process continues till we reach the last sector, by the exit, where the only thing left is to connect them */
class Sector {
public:
    std::pair<int, int> topLeft;
    int rowSize = 6;
    int colSize = 6;
    std::vector<Coord> path;
    Coord exit;

    Sector(Coord entry, Direction dir, int maxRow, int maxCol);
    void generatePath(Direction exitDir, int maxRow, int maxCol);
    Coord getExit() const;

private:
    Coord move(Coord pos, Direction d);
    bool inBounds(Coord pos);
    bool isNearExit(Coord pos, int maxRow);
    Direction pickDirectionWithBias(Direction bias);
};

#endif