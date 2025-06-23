// Project: Maze Problem solving in c++
// Name of an author: Nikolić Dalibor SV13-2023
// Date and time of the last changes: 23.06.2025. 18:47

#ifndef MINOTAUR_HEADER
#define MINOTAUR_HEADER

#include "Type.hpp"

/*  Minotaur-> moving one move at the time, when it reaches Robot, it begins to follow
	one of the goals of the game is to avoid contact with this beast or try killing it :) */
class Minotaur : Type {
private:
	char sign;
	bool status;
};

#endif