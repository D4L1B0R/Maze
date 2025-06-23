// Project: Maze Problem solving in c++
// Name of an author: Nikolić Dalibor SV13-2023
// Date and time of the last changes: 23.06.2025. 18:47

// *** Resolving all the functions that are in charge of navigating and moving throughout the sectors ***

/*  Setors have two main points, exit from previous sector that will be the entrance for the next one
    and same exact exit of previous sector that will be entrance for this one
    each sector is creating unique random path inside, that connects its two exit points    
    but it's using stronger bias to draw a path so it looks more straight, not too much chaotic */

#include "Sector.hpp"

Sector::Sector(Coord entry, Direction dir, int maxRow, int maxCol) {
    switch (dir) {
    case DOWN: topLeft = { entry.first + 1, entry.second - colSize / 2 }; break;
    case UP: topLeft = { entry.first - rowSize, entry.second - colSize / 2 }; break;
    case RIGHT: topLeft = { entry.first - rowSize / 2, entry.second + 1 }; break;
    case LEFT: topLeft = { entry.first - rowSize / 2, entry.second - colSize }; break;
    }

    topLeft.first = std::max(1, std::min(topLeft.first, maxRow - rowSize - 2));
    topLeft.second = std::max(1, std::min(topLeft.second, maxCol - colSize - 2));
}

// Checking if current position is inside the maze
bool Sector::inBounds(Coord pos) {
    return pos.first >= topLeft.first &&
        pos.first < topLeft.first + rowSize &&
        pos.second >= topLeft.second &&
        pos.second < topLeft.second + colSize;
}

// Generating the path inside the sector
void Sector::generatePath(Direction exitDir, int maxRow, int maxCol) {
    Coord current = { topLeft.first + rowSize / 2, topLeft.second + colSize / 2 };
    path.push_back(current);

    while (!isNearExit(current, maxRow)) {
        Coord next = move(current, exitDir);
        if (!inBounds(next)) break;

        path.push_back(next);
        current = next;
    }

    exit = current;
}

Coord Sector::getExit() const {
    return exit;
}

// Making the move based on the current position and direction
Coord Sector::move(Coord pos, Direction d) {
    switch (d) {
    case UP: return { pos.first - 1, pos.second };
    case DOWN: return { pos.first + 1, pos.second };
    case LEFT: return { pos.first, pos.second - 1 };
    case RIGHT: return { pos.first, pos.second + 1 };
    }
    return pos;
}

bool Sector::isNearExit(Coord pos, int maxRow) {
    return pos.first >= maxRow - 3;
}

// This project is using bias to determine the way of going based on the posiiton of the sector's endings
Direction Sector::pickDirectionWithBias(Direction bias) {
    std::vector<std::pair<Direction, int>> options = {
        {UP, 10}, {DOWN, 40}, {LEFT, 25}, {RIGHT, 25}
    };
    int total = 0;
    for (const auto& pair : options) total += pair.second;

    int r = randomNumberGenerator(1, total);
    int acc = 0;
    for (const auto& pair : options) {
        acc += pair.second;
        if (r <= acc) return pair.first;
    }
    return bias;
}