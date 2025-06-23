// Project: Maze Problem solving in c++
// Name of an author: Nikolić Dalibor SV13-2023
// Date and time of the last changes: 23.06.2025. 18:47

#ifndef DYNAMIC_HEADER
#define DYNAMIC_HEADER

#include <unordered_map>
#include "CoordHash.hpp"

class Maze;	// --> include class Maze for use

// Functions for navigating and making the game work, moving the objsects inside the maze
int randomNumberGenerator(int start, int end);
bool sameSector(Coord a, Coord b, int sectorSize = 5);
void moveMinotaur(Maze& maze);
bool moveRobot(char input, Maze& maze);
char getPowerTypeAt(Coord pos, const std::unordered_map<Coord, char>& powerUpTypes);
bool gameLoop(Maze& maze);
void printGame(Maze& maze);
void printActivePowers(Maze& maze);

#endif