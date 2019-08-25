#include "pch.h"

bitmap_image markBorders(bitmap_image image, rgb_t obs, rgb_t frZ, rgb_t bord)
{
	std::vector<coord> mask = { {-1, -1},        {1, -1},
								        
								{-1,  1},		 {1,  1}, 
										 {0, -1},
								{-1,  0},        {1,  0},
										 {0,  1},        };
	coord B, shiftCoord;

	for (B.y = 0; B.y < image.height(); B.y++)
		for (B.x = 0; B.x < image.width(); B.x++)
			if (image.get_pixel(B.x, B.y) == frZ)
				for (auto it : mask)
				{
					shiftCoord = B + it;

					if (coordInImage(image, shiftCoord))
						if (image.get_pixel(shiftCoord.x, shiftCoord.y) == obs)
						{
							image.set_pixel(B.x, B.y, bord);
							borderCoords.push_back(B);
							break;
						}
				}

	return image;
}

std::vector< std::vector<coord> > countContours(bitmap_image markedimage, std::vector<coord> bC)  //bC - borderCoords
{
	std::vector< std::vector<coord> > ccC(1, std::vector<coord>()); //closedContourCoords

	std::vector<coord> mask = { {0, 1}, {-1, 0}, {0, -1}, {1, 0} }; //clockwise
	coord  shiftCoord, bufCoord = bC.back();
	int closedContours = 0;
	bool foundBorderPixel = false;

	while (true)
	{
		bool foundBorderPixel = false;

		for (auto it : mask)
		{
			shiftCoord = bufCoord + it;

			if (coordInImage(markedimage, shiftCoord))
			{
				if (markedimage.get_pixel(shiftCoord.x, shiftCoord.y) == red)
				{
					markedimage.set_pixel(bufCoord.x, bufCoord.y, green);
					ccC[closedContours].push_back(bufCoord);
					foundBorderPixel = true;
					bufCoord = shiftCoord;
					break;
				}

				if (shiftCoord.x == bC.back().x && shiftCoord.y == bC.back().y)
				{
					markedimage.set_pixel(bufCoord.x, bufCoord.y, green);
					ccC[closedContours].push_back(bufCoord);
					closedContours++;
					ccC.push_back(std::vector<coord>());
					break;
				}
			}
		}

		if (!foundBorderPixel)
		{
			bC.pop_back();

			while (!bC.empty() && markedimage.get_pixel(bC.back().x, bC.back().y) != red)
				bC.pop_back();

			if (bC.empty()) break;
			else bufCoord = bC.back();
		}
	}
		
	markedimage.save_image("ContoursOutPutMap3.bmp");

	ccC.pop_back();

	return ccC;
}

bitmap_image fillGapsBetweenWalls(bitmap_image image)
{
	coord B;

	for (int layer = 0; layer < 3; layer++)
	{
		image = markBorders(image, black, white, red);

		for (B.y = 0; B.y < image.height(); B.y++)
			for (B.x = 0; B.x < image.width(); B.x++)
				if (image.get_pixel(B.x, B.y) == red)
					image.set_pixel(B.x, B.y, black);
	}

	for (int layer = 0; layer < 3; layer++)
	{
		image = markBorders(image, white, black, red);

		for (B.y = 0; B.y < image.height(); B.y++)
			for (B.x = 0; B.x < image.width(); B.x++)
				if (image.get_pixel(B.x, B.y) == red)
					image.set_pixel(B.x, B.y, white);
	}
	
	return image;
}

bitmap_image smoothingEdges(bitmap_image image)
{
	coord B, shiftCoord;


	for (int layer = 0; layer < 2; layer++)
	{
		image = markBorders(image, black, white, red);

		for (B.y = 0; B.y < image.height(); B.y++)
			for (B.x = 0; B.x < image.width(); B.x++)
				if (image.get_pixel(B.x, B.y) == red)
					image.set_pixel(B.x, B.y, black);
	}

	std::vector<coord> mask = { {-1, -1},        {1, -1},

								{-1,  1},		 {1,  1},
										 {0, -1},
								{-1,  0},        {1,  0},
										 {0,  1}, };


	int quan_white = 0;

	for (int layer = 0; layer < 2; layer++)
	{
		image = markBorders(image, white, black, red);

		for (B.y = 0; B.y < image.height(); B.y++)
			for (B.x = 0; B.x < image.width(); B.x++)
				if (image.get_pixel(B.x, B.y) == red)
				{
					for (auto it : mask)
					{
						shiftCoord = B + it;

						if (coordInImage(image, shiftCoord))
							if (image.get_pixel(shiftCoord.x, shiftCoord.y) == white)
							{
								quan_white++;

								if (quan_white > 1)
								{
									image.set_pixel(B.x, B.y, blue);
									break;
								}
							}
					}
					quan_white = 0;
				}

		for (B.y = 0; B.y < image.height(); B.y++)
			for (B.x = 0; B.x < image.width(); B.x++)
				if (image.get_pixel(B.x, B.y) == blue)
					image.set_pixel(B.x, B.y, white);
				else
					if (image.get_pixel(B.x, B.y) == red)
						image.set_pixel(B.x, B.y, black);
	}

	return image;
}

bitmap_image intoTwoColorIm(bitmap_image& image)
{
	for (int i = 0; i < image.height(); i++)
		for (int j = 0; j < image.width(); j++)
			if (image.get_pixel(j, i) < d_white)
				image.set_pixel(j, i, black);
			else 
				image.set_pixel(j, i, white);

	return image;
}

bool coordInImage(bitmap_image& image, coord& c)
{
	if (c.x < 0 || c.y < 0 || c.x >= image.width() || c.y >= image.height())
		return false;

	return true;
}

//working with contours

void centerOfTheContour(std::vector< std::vector<coord> >& ccC)
{
	coord sum, center;

	for (int i = 0; i < ccC.size(); i++)
	{
		for (int j = 0; j < ccC[i].size(); j++)
			sum = sum + ccC[i][j];

		center = coord( sum.x / ccC[i].size(), sum.y / ccC[i].size() );
		ccC[i].push_back(center);
		sum = coord();
	}

}
