#ifndef ROBOT_HEADER
#define ROBOT_HEADER

#include "Type.hpp"
#include "Object.hpp"
#include <vector>

class Robot : Type {
private:
	char sign;
	bool status;
	std::vector<Object> magic;
public:
};

#endif