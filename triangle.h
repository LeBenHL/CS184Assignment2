#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "surface.h"
#include "three_d_vector.h";
#include <iostream>
#include <Eigen/Dense>
using namespace Eigen;

class Triangle: public Surface{
	public:
		//vertices
		ThreeDVector* a;
		ThreeDVector* b;
		ThreeDVector* c;
		ThreeDVector* normal;

		Triangle(ThreeDVector* _a, ThreeDVector* _b, ThreeDVector* _c);
		ThreeDVector* get_normal(ThreeDVector* surface_point);
		bool hit(Ray* ray, Record* record);

		~Triangle();

};

#endif
