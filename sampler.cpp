#include "sampler.h"

Sampler::Sampler(int i_w, int i_h){
	image_width = i_w;
	image_height = i_h;

	LL = new ThreeDVector(0, 0, 0);
	UR = new ThreeDVector(1, 1, 0);
}

vector<ThreeDVector*>  Sampler::get_sample(int x, int y, int grid_size){
	vector<ThreeDVector*> samples; 
	for(int i = 0; i < grid_size; i++){
		for(int j = 0; j < grid_size; j++){
			float new_x = (UR->x - LL->x)/image_width * (x + 0.5); 
			float new_y = (UR->y - LL->y)/image_height * (y + 0.5); 
			float new_z = LL->z;
			ThreeDVector* sample = new ThreeDVector(new_x, new_y, new_z);
			samples.push_back(sample);
		}
	}
	return samples;
}

