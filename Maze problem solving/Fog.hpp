// Project: Maze Problem solving in c++
// Name of an author: Nikolić Dalibor SV13-2023
// Date and time of the last changes: 23.06.2025. 18:47

#ifndef FOG_HEADER
#define FOG_HEADER

#include "Object.hpp"

// ** FOG OF THE DAMNED **
/*  A cursed mist that seeps from the dark crevices of the maze.
    When triggered, it engulfs the Robot’s vision, reducing sight to a mere 3x3 grid.
    For several moves, every direction becomes uncertain — escape grows far more dangerous. */

class Fog : public Object {
public:
    Fog() : Object('F') {}
};

#endif