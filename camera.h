#ifndef CAMERA_H
#define CAMERA_H
#include "three_d_vector.h"
#include "ray.h"
#include <iostream>
#include <vector>

using namespace std;

class Camera{
	public:
		// Position is the Camera is in
		ThreeDVector* look_from;
		// Where am I looking at
		ThreeDVector* look_at;
		// My UpVector
		ThreeDVector* up;
		// Focal Length (How far away am I from image plane)
		float image_plane_length;
		float focal_length;

		bool dof;

		//Orthonormal basis
		ThreeDVector* u;
		ThreeDVector* v;
		ThreeDVector* w;


		float fov;
		Camera(float, float, float, float, float, float, float, float, float, float);
		~Camera();
		vector<Ray*> get_view_rays(ThreeDVector*);


};

#endif