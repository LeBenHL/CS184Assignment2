#ifndef RAY_H
#define RAY_H

#include <iostream>

#include "three_d_vector.h"

class Ray{
	public:
		ThreeDVector* direction;
		ThreeDVector* position;
		Ray(ThreeDVector* position, ThreeDVector* direction);
		char* repr();
		~Ray();
};

#endif