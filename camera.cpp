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

    delete pos_w;
}

Ray* Camera::get_view_ray(ThreeDVector* screen_pos) {
    ThreeDVector* origin = this->look_from;
   
    ThreeDVector* scaled_w = this->w->scalar_multiply(-(this->focal_length));
    ThreeDVector* scaled_u = this->u->scalar_multiply(screen_pos->x);
    ThreeDVector* scaled_v = this->v->scalar_multiply(screen_pos->y);
    ThreeDVector* w_plus_u = scaled_w->vector_add(scaled_u);
   
    ThreeDVector* w_plus_u_plus_v = w_plus_u->vector_add(scaled_v);

    delete scaled_w;
    delete scaled_u;
    delete scaled_v;
    delete w_plus_u;

    return new Ray(origin, w_plus_u_plus_v, 0, numeric_limits<float>::infinity());

}

Camera::~Camera() {
    delete look_from;
    delete look_at;
    delete up;
    delete u;
    delete v;
    delete w;
}