// Project: Maze Problem solving in c++
// Name of an author: Nikolić Dalibor SV13-2023
// Date and time of the last changes: 23.06.2025. 18:47

#ifndef TYPE_HEADER
#define TYPE_HEADER

// Types of block in the maze-> Mintaur (M), Robot (R), Wall (#), Path (.), Entrance (U), Exit (I), Magic Objects (P)
class Type {
protected:
	char sign;
public:
	Type() : sign(' ') {};
	Type(char c) : sign(c) {};
	virtual ~Type() = default;
	virtual char getChar() const { return sign; }
};

#endif