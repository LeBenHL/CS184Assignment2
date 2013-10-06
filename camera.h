#ifndef CAMERA_H
#define CAMERA_H
#include "three_d_vector.h"
#include "ray.h"
#include <iostream>

class Camera{
	public:
		// Position is the Camera is in
		ThreeDVector* look_from;
		// Where am I looking at
		ThreeDVector* look_at;
		// My UpVector
		ThreeDVector* up;
		// Focal Length (How far away am I from image plane)
		float focal_length;

		//Orthonormal basis
		ThreeDVector* u;
		ThreeDVector* v;
		ThreeDVector* w;


		float fov;
		Camera(float, float, float, float, float, float, float, float, float, float);
		~Camera();
		Ray* get_view_ray(ThreeDVector*);


};

#endif