#ifndef HAMMER_HEADER
#define HAMMER_HEADER

#include "Object.hpp"

// **HAMMER OF EARTH’S WRATH**
// A relic of immense force, said to be able to shatter the very bones of the maze itself.
// The Robot may use it to destroy a single wall of his choosing,
// opening an otherwise impossible path forward. Use with strategy, not rage.

class Hammer : public Object {
public:
    Hammer() : Object('B') {}
};

#endif