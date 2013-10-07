#include "sampler.h"

#include <cmath>

Sampler::Sampler(int i_w, int i_h, float fov, float focal_length) {
	image_width = i_w;
	image_height = i_h;

	float plane_height = tan(fov/2) * 2 * focal_length;
	float plane_width = plane_height * (image_width / image_height);


	LL = new ThreeDVector(-plane_width/2, -plane_height/2, 0);
	UR = new ThreeDVector(plane_width/2, plane_height/2, 0);
}

vector<ThreeDVector*>  Sampler::get_sample(int x, int y, int grid_size){
	vector<ThreeDVector*> samples; 
	for(int i = 0; i < grid_size; i++){
		for(int j = 0; j < grid_size; j++){
			float new_x = (UR->x - LL->x)/image_width * (x + 0.5) + LL->x; 
			float new_y = (UR->y - LL->y)/image_height * (y + 0.5) + LL->y; 
			float new_z = LL->z;
			ThreeDVector* sample = new ThreeDVector(new_x, new_y, new_z);
			samples.push_back(sample);
		}
	}
	return samples;
}

