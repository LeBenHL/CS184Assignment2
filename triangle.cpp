#include "triangle.h"

Triangle::Triangle(ThreeDVector* _a, ThreeDVector* _b, ThreeDVector* _c){
	a = _a;
	b = _b;
	c = _c;
	ThreeDVector* b_minus_a = _b->vector_subtract(_a);
	ThreeDVector* c_minus_a = _c->vector_subtract(_a);
	normal = b_minus_a->cross_product(c_minus_a);
	normal->normalize_bang();
	delete b_minus_a;
	delete c_minus_a; 
}

bool Triangle::hit(Ray* ray, Record* record){

	//matrix A
	float a = this->a->x - this->b->x;
	float b = this->a->y - this->b->y;
	float c = this->a->z - this->b->z;
	float d = this->a->x - this->c->x;
	float e = this->a->y - this->c->y;
	float f = this->a->z - this->c->z;
	float g = ray->direction->x;
	float h = ray->direction->y;
	float i = ray->direction->z;

	//vector b
	float j = this->a->x - ray->position->x;
	float k = this->a->y - ray->position->y;
	float l = this->a->z - ray->position->z;
	
	//reuse floats to reduce number of operations
	float ei_minus_hf = e*i-h*f;
	float gf_minus_di = g*f-d*i;
	float dh_minus_eg = d*h-e*g;
	float ak_minus_jb = a*k-j*b;
	float jc_minus_al = j*c-a*l;
	float bl_minus_kc = b*l-k*c;

	//determinant of A
	float m = a*ei_minus_hf + b*gf_minus_di + c*dh_minus_eg;

	float t = f*ak_minus_jb + e*jc_minus_al + d*bl_minus_kc;
	t /= -m;

	if (t < ray->t_min || t > ray->t_max) {
		return false;
	}

	float gamma = i*ak_minus_jb + h*jc_minus_al + g*bl_minus_kc;
	gamma /= m;

	if(gamma < 0 || gamma > 1){
		return false;
	}

	float beta = j*ei_minus_hf + k*gf_minus_di + l*dh_minus_eg;
	beta /= m;

	if(beta < 0 || beta > (1-gamma)){
		return false;
	}
	record->t_hit = t;
	return true;
}

ThreeDVector* Triangle::get_normal(ThreeDVector* surface_point, ThreeDVector* view_vector) {
	ThreeDVector* negative_normal = this->normal->scalar_multiply(-1);
	float negative_normal_dot_product = negative_normal->dot_product(view_vector);
	float normal_dot_product = this->normal->dot_product(view_vector);

	if (normal_dot_product > negative_normal_dot_product) {
		return negative_normal;
	} else {
		delete negative_normal;
		return this->normal->clone();
	}
}

Triangle::~Triangle(){
	delete a;
	delete b;
	delete c;
	delete normal;
}