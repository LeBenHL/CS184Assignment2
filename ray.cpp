#include "ray.h"

Ray::Ray(float x, float y, float z, float x_d, float y_d, float z_d){
	position = new ThreeDVector(x, y, z);
	direction = new ThreeDVector(x_d, y_d, z_d);
}

Ray::~Ray(){
	delete position;
	delete direction;
}