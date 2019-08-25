#ifndef PCH_H
#define PCH_H

#include <iostream>
#include <vector>
#include "bitmap_image.hpp"

struct coord
{
	int x, y;

	coord() : x(0), y(0) {}

	coord(int arg1, int arg2)
	{
		x = arg1;
		y = arg2;
	}

	coord& operator=(const coord& arg)
	{
		this->x = arg.x;
		this->y = arg.y;

		return *this;
	}

	friend coord operator+(const coord& arg1, const coord arg2)
	{
		return coord{ arg1.x + arg2.x, arg1.y + arg2.y };
	}
};

bitmap_image markBorders(bitmap_image, rgb_t, rgb_t, rgb_t);

std::vector< std::vector<coord> > countContours(bitmap_image, std::vector<coord>);

bitmap_image fillGapsBetweenWalls(bitmap_image);

bitmap_image smoothingEdges(bitmap_image);

bitmap_image intoTwoColorIm(bitmap_image&);

bool coordInImage(bitmap_image&, coord&);

//working with contours

void centerOfTheContour(std::vector< std::vector<coord> >&);

//constants

const rgb_t 
red{ 255, 0, 0 },
black{ 0, 0, 0 },
white{ 255, 255, 255 },
d_white{ 250, 250, 250 },
yellow{ 255, 255, 0 },
green{ 0, 255, 0 },
blue{ 0, 0, 255 };

//general var

extern std::vector<coord>   borderCoords,
borderInCoords,
borderOutCoords;

#endif 
