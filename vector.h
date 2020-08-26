#pragma once

struct vec2
{
	int x;
	int y;
	vec2(int _x = 0, int _y = 0) : x(_x), y(_y) {}
	vec2& operator=(vec2 rhs) {
		x = rhs.x;
		y = rhs.y;
		return *this;
	}
};
