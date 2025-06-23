#ifndef TYPE_HEADER
#define TYPE_HEADER

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