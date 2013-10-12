#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "surface.h"
#include "three_d_vector.h"

class Triangle: public Surface{
	public:
		//vertices
		ThreeDVector* a;
		ThreeDVector* b;
		ThreeDVector* c;
		ThreeDVector* normal;

		Triangle(ThreeDVector* _a, ThreeDVector* _b, ThreeDVector* _c);
		ThreeDVector* get_normal(ThreeDVector* surface_point, ThreeDVector* view_vector);
		bool hit(Ray* ray, Record* record);

		~Triangle();

};

#endif
