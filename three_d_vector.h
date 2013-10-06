#ifndef THREEDVECTOR_H
#define THREEDVECTOR_H

class ThreeDVector{
	public:
		float x;
		float y;
		float z;
		ThreeDVector(float=0, float=0, float=0);
		float magnitude();
		ThreeDVector* normalize();
		float dot_product(ThreeDVector*);
		ThreeDVector* scalar_multiply(float k);
		void scalar_multiply_bang(float k);
		ThreeDVector* vector_add(ThreeDVector*);
		void vector_add_bang(ThreeDVector*);
		ThreeDVector* vector_subtract(ThreeDVector*);
		ThreeDVector* vector_multiply(ThreeDVector*);
		ThreeDVector* cross_product(ThreeDVector*);
		ThreeDVector* clone();
		char* repr();
};

#endif