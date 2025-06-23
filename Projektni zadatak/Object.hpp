#ifndef OBJECT_HEADER
#define OBJECT_HEADER

#include "Type.hpp"

class Object : public Type {
protected:
    char effectCode;  // 'F', 'S', 'H', 'B'
public:
    Object(char code) : Type('P'), effectCode(code) {}
    char getEffectCode() const { return effectCode; }
};

#endif