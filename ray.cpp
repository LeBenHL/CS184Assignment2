#include "ray.h"
#include <stdio.h>

using namespace std;

Ray::Ray(ThreeDVector* _position, ThreeDVector* _direction, long double _t_min, long double _t_max) {
	position = _position;
	direction = _direction;
	t_min = _t_min;
	t_max = _t_max;

	inv_direction = new ThreeDVector(1.0/direction->x, 1.0/direction->y, 1.0/direction->z);
}

char* Ray::repr() {
	char* buffer = new char[1000];
	char* position_repr = this->position->repr();
	char* direction_repr = this->direction->repr();
	sprintf(buffer, "<ThreeDVector, position = %s, direction = %s>", position_repr, direction_repr);
	delete direction_repr;
	delete position_repr;
	return buffer;
}

ThreeDVector* Ray::point_at(long double t){
	ThreeDVector* t_times_direction = this->direction->scalar_multiply(t);
	ThreeDVector* point = this->position->vector_add(t_times_direction);
	delete t_times_direction;
	return point;
}


Ray::~Ray(){
	delete position;
	delete direction;
	delete inv_direction;
}