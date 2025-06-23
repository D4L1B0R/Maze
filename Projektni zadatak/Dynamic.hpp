#ifndef DYNAMIC_HEADER
#define DYNAMIC_HEADER

#include "CoordHash.hpp"
#include <unordered_map>

class Maze;
extern bool highlightPath;

int randomNumberGenerator(int start, int end);
bool sameSector(Coord a, Coord b, int sectorSize = 5);
Coord bfsTowards(Coord start, Coord goal, Maze& maze);
void moveMinotaur(Maze& maze);
bool moveRobot(char input, Maze& maze);
char getPowerTypeAt(Coord pos, const std::unordered_map<Coord, char>& powerUpTypes);
void useHammer(char input, Maze& maze);
bool gameLoop(Maze& maze);

#endif