// Project: Maze Problem solving in c++
// Name of an author: Nikolić Dalibor SV13-2023
// Date and time of the last changes: 23.06.2025. 18:47

#ifndef OBJECT_HEADER
#define OBJECT_HEADER

#include "Type.hpp"

// Member of the class Type, representing magic objects
class Object : public Type {
protected:
    char effectCode;  // 'F', 'S', 'H', 'B'
public:
    Object(char code) : Type('P'), effectCode(code) {}
    char getEffectCode() const { return effectCode; }
};

#endif