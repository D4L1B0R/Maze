#ifndef SWORD_HEADER
#define SWORD_HEADER

#include "Object.hpp"

// **SWORD OF THE LOST WARRIOR OF RUNETERRA**
// A legendary blade once wielded by the ancient protector of the Maze.
// If the Robot wields this sword, the deadly Minotaur shall fall before its might.
// Instead of meeting a certain death, the Robot strikes down the beast and survives.

class Sword : public Object {
public:
    Sword() : Object('S') {}
};

#endif