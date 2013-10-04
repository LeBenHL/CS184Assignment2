#ifndef CAMERA_H
#define CAMERA_H
#include "three_d_vector.h"
#include <iostream>

class Camera{
	public:
		// Position is the Camera is in
		ThreeDVector* look_from;
		// Where am I looking at
		ThreeDVector* look_at;
		// My UpVector
		ThreeDVector* up;


		float fov;
		Camera(float, float, float, float, float, float, float, float, float, float);
		~Camera();

};

#endif