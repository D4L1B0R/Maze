#ifndef SECTOR_HEADER
#define SECTOR_HEADER

#include <vector>
#include <utility>
#include <unordered_set>
#include "Dynamic.hpp"
#include "CoordHash.hpp"

enum Direction { UP = 1, DOWN = 2, RIGHT = 3, LEFT = 4 };

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