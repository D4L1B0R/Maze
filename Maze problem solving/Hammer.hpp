// Project: Maze Problem solving in c++
// Name of an author: Nikolić Dalibor SV13-2023
// Date and time of the last changes: 23.06.2025. 18:47

#ifndef HAMMER_HEADER
#define HAMMER_HEADER

#include "Object.hpp"

// ** HAMMER OF EARTH’S WRATH **
/*  A relic of immense force, said to be able to shatter the very bones of the maze itself.
    The Robot may use it to destroy a single wall of his choosing,
    opening an otherwise impossible path forward. Use with strategy, not rage.  */

class Hammer : public Object {
public:
    Hammer() : Object('B') {}
};

#endif