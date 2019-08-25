# Border Maker
# This program require C++ Bitmap Library (only "bitmap_image.hpp")
  
	Short description
Program converts image into 2 color image, depending on d_white global constant variable (which is set to d_white{ 250, 250, 250 }). Then marks the borders, count closed border contours, compute the centers of closed border contours. There are some image transitions outputs are created.

Initial file is 24-bit bmp format named colorIm.bmp. Image have to be within frames with color < d_white.

