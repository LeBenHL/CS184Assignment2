#ifndef SAMPLER_H
#define SAMPLER_H

#include <vector>
#include "three_d_vector.h"

using namespace std;

class Sampler{
	public:
		int image_width;
		int image_height;

		ThreeDVector* LL;
		ThreeDVector* UR;

		Sampler(int, int);
		vector<ThreeDVector*> get_sample(int, int, int);
};

#endif