#ifndef PIXEL_H
#define PIXEL_H

#include "three_d_vector.h"
#include "camera.h"

class Pixel{
	public:
		int x;
		int y;
		Camera* camera;
		Pixel(int, int, Camera*);
		~Pixel();
		ThreeDVector* get_color();
};

#endif