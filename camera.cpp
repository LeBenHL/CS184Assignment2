#include "camera.h"

using namespace std;

Camera::Camera(float lf_x, float lf_y, float lf_z, float la_x, float la_y, float la_z, float up_x, float up_y, float up_z, float field_of_view) {
    look_from = new ThreeDVector(lf_x, lf_y, lf_z);
    look_at = new ThreeDVector(la_x - lf_x, la_y - lf_y, la_z -lf_z);
    up = new ThreeDVector(up_x, up_y, up_z);
    fov = field_of_view;
    dof = false;

    ThreeDVector* pos_w = look_at->normalize();

    //Solve for our Orthonormal basis
    w = pos_w->scalar_multiply(-1);
    u = up->cross_product(w);
    u->normalize_bang();
    v = w->cross_product(u);

    delete pos_w;
}

vector<Ray*> Camera::get_view_rays(ThreeDVector* screen_pos) {
    vector<Ray*> rays;
    ThreeDVector* origin = this->look_from->clone();
    ThreeDVector* scaled_w = this->w->scalar_multiply(-(this->image_plane_length));
    ThreeDVector* scaled_u = this->u->scalar_multiply(screen_pos->x);
    ThreeDVector* scaled_v = this->v->scalar_multiply(screen_pos->y);
    ThreeDVector* w_plus_u = scaled_w->vector_add(scaled_u);
   
    ThreeDVector* w_plus_u_plus_v = w_plus_u->vector_add(scaled_v);

    delete scaled_w;
    delete scaled_u;
    delete scaled_v;
    delete w_plus_u;

    Ray* unperturbed_ray = new Ray(origin, w_plus_u_plus_v, 0, numeric_limits<float>::infinity());

    if (this->dof) {
        // DOF inspired by http://ray-tracer-concept.blogspot.com/2011/12/depth-of-field.html
        ThreeDVector* point_aimed = unperturbed_ray->point_at(this->focal_length);
        for (int di =0; di < 25; di++) { // shooting 25 random rays
            ThreeDVector* camera_pos = this->look_from->clone();
            //Perturb the Origin a bit
            float camera_size = 0.1;
            float scale_u = (float(rand()) / RAND_MAX * camera_size) - camera_size/2;
            float scale_v = (float(rand()) / RAND_MAX * camera_size) - camera_size/2;
            ThreeDVector* perturb_u = this->u->scalar_multiply(scale_u);
            ThreeDVector* perturb_v = this->v->scalar_multiply(scale_v);
            camera_pos->vector_add_bang(perturb_u);
            camera_pos->vector_add_bang(perturb_v);

            ThreeDVector* direction = point_aimed->vector_subtract(camera_pos);
            rays.push_back(new Ray(camera_pos, direction, 0, numeric_limits<float>::infinity()));
        }

        delete point_aimed;
        delete unperturbed_ray;
    } else {
        rays.push_back(unperturbed_ray);
    }

    return rays;

}

Camera::~Camera() {
    delete look_from;
    delete look_at;
    delete up;
    delete u;
    delete v;
    delete w;
}