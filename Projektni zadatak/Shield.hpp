#ifndef SHIELD_HEADER
#define SHIELD_HEADER

#include "Object.hpp"

// **ANGELIC SHIELD OF AEONS**
// Forged by the forgotten sentinels of the Inner Walls, this shield grants a second chance.
// When the Robot encounters the Minotaur, the shield absorbs the fatal blow,
// letting the Robot narrowly escape — but only once. Use it wisely.

class Shield : public Object {
public:
    Shield() : Object('H') {}
};

#endif