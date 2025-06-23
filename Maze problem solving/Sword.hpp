// Project: Maze Problem solving in c++
// Name of an author: Nikolić Dalibor SV13-2023
// Date and time of the last changes: 23.06.2025. 18:47

#ifndef SWORD_HEADER
#define SWORD_HEADER

#include "Object.hpp"

// ** SWORD OF THE LOST WARRIOR OF RUNETERRA **
/*  A legendary blade once wielded by the ancient protector of the Maze.
    If the Robot wields this sword, the deadly Minotaur shall fall before its might.
    Instead of meeting a certain death, the Robot strikes down the beast and survives.  */

class Sword : public Object {
public:
    Sword() : Object('S') {}
};

#endif