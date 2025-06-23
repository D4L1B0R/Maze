// Project: Maze Problem solving in c++
// Name of an author: Nikolić Dalibor SV13-2023
// Date and time of the last changes: 23.06.2025. 18:47

#ifndef ROBOT_HEADER
#define ROBOT_HEADER

#include "Type.hpp"
#include "Object.hpp"
#include <vector>

/*  Robot Robert -> moving one move at the time, afraid of the darkness, but brave little fellow
	he is trying to escape from the destiny and reach the end of an maze with all of his screws */
class Robot : Type {
private:
	char sign;
	bool status;
	std::vector<Object> magic;
};

#endif