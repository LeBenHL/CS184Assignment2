#ifndef RAY_H
#define RAY_H

#include <iostream>

#include "three_d_vector.h"

using namespace std;

struct Record{
    long double t_hit; 
    ThreeDVector* position_hit;
    ThreeDVector* untransformed_position_hit;
}; 

class Ray{
	public:
		ThreeDVector* direction;
		ThreeDVector* position;
		long double t_min;
		long double t_max;

		//For AABB hit
		ThreeDVector* inv_direction;

		Ray(ThreeDVector* position, ThreeDVector* direction, long double t_min, long double t_max);
		char* repr();
		ThreeDVector* point_at(long double t);
		~Ray();
};

#endif