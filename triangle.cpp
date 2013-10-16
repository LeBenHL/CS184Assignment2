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
	long double a = this->a->x - this->b->x;
	long double b = this->a->y - this->b->y;
	long double c = this->a->z - this->b->z;
	long double d = this->a->x - this->c->x;
	long double e = this->a->y - this->c->y;
	long double f = this->a->z - this->c->z;
	long double g = ray->direction->x;
	long double h = ray->direction->y;
	long double i = ray->direction->z;

	//vector b
	long double j = this->a->x - ray->position->x;
	long double k = this->a->y - ray->position->y;
	long double l = this->a->z - ray->position->z;
	
	//reuse long doubles to reduce number of operations
	long double ei_minus_hf = e*i-h*f;
	long double gf_minus_di = g*f-d*i;
	long double dh_minus_eg = d*h-e*g;
	long double ak_minus_jb = a*k-j*b;
	long double jc_minus_al = j*c-a*l;
	long double bl_minus_kc = b*l-k*c;

	//determinant of A
	long double m = a*ei_minus_hf + b*gf_minus_di + c*dh_minus_eg;

	long double t = f*ak_minus_jb + e*jc_minus_al + d*bl_minus_kc;
	t /= -m;

	if (t < ray->t_min || t > ray->t_max) {
		return false;
	}

	long double gamma = i*ak_minus_jb + h*jc_minus_al + g*bl_minus_kc;
	gamma /= m;

	if(gamma < 0 || gamma > 1){
		return false;
	}

	long double beta = j*ei_minus_hf + k*gf_minus_di + l*dh_minus_eg;
	beta /= m;

	if(beta < 0 || beta > (1-gamma)){
		return false;
	}

	ThreeDVector* point_hit = ray->point_at(t);

	record->t_hit = t;
	record->position_hit = point_hit;
	record->untransformed_position_hit = point_hit->clone();

	return true;
}

ThreeDVector* Triangle::get_normal(ThreeDVector* surface_point, ThreeDVector* view_vector) {
	ThreeDVector* negative_normal = this->normal->scalar_multiply(-1);
	long double negative_normal_dot_product = negative_normal->dot_product(view_vector);
	long double normal_dot_product = this->normal->dot_product(view_vector);

	if (normal_dot_product > negative_normal_dot_product) {
		return negative_normal;
	} else {
		delete negative_normal;
		return this->normal->clone();
	}
}

void Triangle::apply_transformation(Eigen::Matrix4f transformation) {
    Eigen::Vector4f old_a = Eigen::Vector4f(this->a->x, this->a->y, this->a->z, 1);
    Eigen::Vector4f old_b = Eigen::Vector4f(this->b->x, this->b->y, this->b->z, 1);
    Eigen::Vector4f old_c = Eigen::Vector4f(this->c->x, this->c->y, this->c->z, 1);
    Eigen::Vector4f new_a = transformation * old_a;
    Eigen::Vector4f new_b = transformation * old_b;
    Eigen::Vector4f new_c = transformation * old_c;

    delete this->a;
    delete this->b;
    delete this->c;

    this->a = new ThreeDVector(new_a[0], new_a[1], new_a[2]);
    this->b = new ThreeDVector(new_b[0], new_b[1], new_b[2]);
    this->c = new ThreeDVector(new_c[0], new_c[1], new_c[2]);
}

void Triangle::get_bounds() {

	long double x_min = min(this->a->x, min(this->b->x, this->c->x));
	long double x_max = max(this->a->x, max(this->b->x, this->c->x));
	long double y_min = min(this->a->y, min(this->b->y, this->c->y));
	long double y_max = max(this->a->y, max(this->b->y, this->c->y));
	long double z_min = min(this->a->z, min(this->b->z, this->c->z));
	long double z_max = max(this->a->z, max(this->b->z, this->c->z));

	this->bounds = new Bounds(x_min, x_max, y_min, y_max, z_min, z_max);
}

Triangle::~Triangle(){
	delete a;
	delete b;
	delete c;
	delete normal;
}