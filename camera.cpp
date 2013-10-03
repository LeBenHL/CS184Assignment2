#include "camera.h"

using namespace std;

Camera::Camera(float lf_x, float lf_y, float lf_z, float la_x, float la_y, float la_z, float up_x, float up_y, float up_z, float field_of_view) {
	look_from = new ThreeDVector(lf_x, lf_y, lf_z);
	look_at = new ThreeDVector(la_x, la_y, la_z);
	up = new ThreeDVector(up_x, up_y, up_z);
	fov = field_of_view;

	ThreeDVector* pos_w = look_at->normalize();

	//Solve for our Orthonormal basis
	v = up->normalize();
	w = pos_w->scalar_multiply(-1);
	u = v->cross_product(w);

	cout << v->repr() << endl;
	cout << w->repr() << endl;
	cout << u->repr() << endl;

	delete pos_w;
}

Camera::~Camera() {
	delete look_from;
	delete look_at;
	delete up;
	delete u;
	delete v;
	delete w;
}