#ifndef WALL_HEADER
#define WALL_HEADER

#include "Type.hpp"

class Wall : public Type {
private:
	char sign;
public:
	Wall() { this->sign = '#'; };
	~Wall() {};
};

#endif