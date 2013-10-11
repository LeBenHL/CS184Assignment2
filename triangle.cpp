#include "triangle.h"

Triangle::Triangle(ThreeDVector* _a, ThreeDVector* _b, ThreeDVector* _c){
	a = _a;
	b = _b;
	b = _c;
	ThreeDVector* b_minus_a = _b->vector_subtract(_a);
	ThreeDVector* c_minus_a = _c->vector_subtract(_a);
	normal = b_minus_a->cross_product(c_minus_a);
	normal->normalize_bang();
	delete b_minus_a;
	delete c_minus_a; 
}

bool Triangle::hit(Ray* ray, Record* record){
	Matrix3f A;
	Vector3f b;

	float a1 = this->a->x - this->b->x;
	float a2 = this->a->x - this->c->x;
	float a3 = ray->direction->x;
	float a4 = this->a->y - this->b->y;
	float a5 = this->a->y - this->c->y;
	float a6 = ray->direction->y;
	float a7 = this->a->z - this->b->z;
	float a8 = this->a->z - this->c->z;
	float a9 = ray->direction->z;

	A << a1,a2,a3,  a4,a5,a6,  a7,a8,a9;

	float b1 = this->a->x - ray->position->x;
	float b2 = this->a->y - ray->position->y;
	float b3 = this->a->z - ray->position->z;

	b << b1, b2, b3;

	Vector3f x = A.colPivHouseholderQr().solve(b);

	float beta = x[0];
	float gamma = x[1];
	float t = x[2];

	if (t < ray->t_min || t > ray->t_max) {
		return false;
	}
	if(gamma < 0 || gamma > 1){
		return false;
	}
	if(beta < 0 || beta > (1-gamma)){
		return false;
	}
	record->t_hit = t;
	return true;
}

ThreeDVector* Triangle::get_normal(ThreeDVector* surface_point) {
	return normal->clone();
}

Triangle::~Triangle(){
	delete a;
	delete b;
	delete c;
}