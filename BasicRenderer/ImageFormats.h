#pragma once


struct RGB888
{
	int r = 0;
	int g = 0;
	int b = 0;
	
public:
	RGB888() : r(0), g(0), b(0) {}
	RGB888(const RGB888& other) : r(other.r), g(other.g), b(other.b) {}
	RGB888(int or, int og, int ob) : r(or), g(og), b(ob) {}

	RGB888& operator=(RGB888& other);
};