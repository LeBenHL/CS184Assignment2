#ifndef RAY_H
#define RAY_H

#include <iostream>

#include "three_d_vector.h"

using namespace std;

struct Record{
    float t_intersect; 
    ThreeDVector* intersect_point;
}; 

class Ray{
	public:
		ThreeDVector* direction;
		ThreeDVector* position;
		float t_min;
		float t_max;
		Ray(ThreeDVector* position, ThreeDVector* direction, float t_min, float t_max);
		char* repr();
		~Ray();
};

#endif