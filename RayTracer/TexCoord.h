#pragma once
class TexCoord
{
private:
	int x;
	int y;

public:
	TexCoord() { x = 0; y = 0; }
	TexCoord(int x_, int y_) {
		x = x_;
		y = y_;
	}
};

