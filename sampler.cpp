#include "sampler.h"

#include <cmath>
#include <iostream>
using namespace std;

Sampler::Sampler(int i_w, int i_h, float fov, float image_plane_length) {
	image_width = i_w;
	image_height = i_h;

	extern float PI;

	float plane_height = tan((fov/2) * PI/180) * 2 * image_plane_length;
	float plane_width = plane_height * (float(image_width) / image_height);


	LL = new ThreeDVector(-plane_width/2, -plane_height/2, 0);
	UR = new ThreeDVector(plane_width/2, plane_height/2, 0);
}

vector<ThreeDVector*>  Sampler::get_sample(int x, int y, int grid_size){
	vector<ThreeDVector*> samples; 
	for(int p = 0; p < grid_size; p++){
		for(int q = 0; q < grid_size; q++){
			float e1;
			float e2;
			if (grid_size == 1) {
				e1 = 0.5;
				e2 = 0.5;
			} else {
				e1 = float(rand()) / RAND_MAX;
				e2 = float(rand()) / RAND_MAX;
			}
			float new_x = (UR->x - LL->x)/image_width * (x + (p + e1)/grid_size) + LL->x; 
			float new_y = (UR->y - LL->y)/image_height * (y + (q + e2)/grid_size) + LL->y; 
			float new_z = LL->z;
			ThreeDVector* sample = new ThreeDVector(new_x, new_y, new_z);
			samples.push_back(sample);
		}
	}
	return samples;
}

