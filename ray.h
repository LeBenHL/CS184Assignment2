#ifndef RAY_H
#define RAY_H

#include <iostream>

#include "three_d_vector.h"

using namespace std;

struct Record{
    float t_hit; 
    ThreeDVector* position_hit;
    ThreeDVector* untransformed_position_hit;
}; 

class Ray{
	public:
		ThreeDVector* direction;
		ThreeDVector* position;
		float t_min;
		float t_max;
		Ray(ThreeDVector* position, ThreeDVector* direction, float t_min, float t_max);
		char* repr();
		ThreeDVector* point_at(float t);
		~Ray();
};

#endif