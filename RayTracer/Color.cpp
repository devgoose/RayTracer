#include <algorithm>

#include "Color.h"

Color::Color(const Color& c) {
	r = c.r;
	g = c.g;
	b = c.b;
}

Color Color::operator+(const Color& c) {
	return Color(r + c.r, g + c.g, b + c.b);
}

Color Color::operator*(const float c) {
	return Color(r * c, g * c, b * c);
}

Color Color::operator*(const Color& c) {
	return Color(r * c.r, g * c.g, b * c.b);
}

void Color::clamp() {
	r = std::min((float)1.0, r);
	g = std::min((float)1.0, g);
	b = std::min((float)1.0, b);
}

std::ostream& operator<<(std::ostream& out, const Color& color) {
	out << (int)(color.r*255) << " " << (int)(color.g*255) << " " << (int)(color.b*255);
	return out;
}