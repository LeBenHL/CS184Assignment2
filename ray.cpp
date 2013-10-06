#include "ray.h"

using namespace std;

Ray::Ray(ThreeDVector* _position, ThreeDVector* _direction) {
	position = _position;
	direction = _direction;
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

Ray::~Ray(){
	delete position;
	delete direction;
}