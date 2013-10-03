#ifndef RAY_H
#define RAY_H

#include "three_d_vector.h"

class Ray{
	public:
		ThreeDVector* direction;
		ThreeDVector* position;
		Ray(float, float, float, float, float, float);
		~Ray();
};

#endif