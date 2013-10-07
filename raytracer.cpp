#include "raytracer.h"

RayTracer::RayTracer(vector<Surface*> _surfaces, vector<Light*> _lights) {
	surfaces = _surfaces;
	lights = _lights;
}

ThreeDVector* RayTracer::trace(Ray* ray) {
	bool hit = false;
	Surface* first_hit;
	Record* record = new Record();
	for(vector<Surface*>::iterator it = this->surfaces.begin(); it != this->surfaces.end(); ++it) {
		Surface* surface = *it;
    	if (surface->hit(ray, record)) {
    		hit = true;
    		first_hit = surface;
    		ray->t_max = record->t_hit;
    	} 
	}

	if (hit) {
		ThreeDVector* point_hit = ray->point_at(record->t_hit);
		ThreeDVector* normal = first_hit->get_normal(point_hit);

		ThreeDVector* pixel_color = new ThreeDVector(0, 0, 0);

		extern ThreeDVector* global_ambient;
		ThreeDVector* diffuse_color = this->calculate_diffuse(first_hit, this->lights, normal, point_hit);
		ThreeDVector* negative_ray_direction = ray->direction->scalar_multiply(-1);
		negative_ray_direction->normalize_bang();
		ThreeDVector* specular_color = this->calculate_specular(first_hit, this->lights, normal, point_hit, negative_ray_direction); 
		pixel_color->vector_add_bang(global_ambient);
		pixel_color->vector_add_bang(diffuse_color);
		pixel_color->vector_add_bang(specular_color);
		delete negative_ray_direction;
		delete diffuse_color;
		return pixel_color;
	} else {
		//Return background color
		return new ThreeDVector(0, 0, 0);
	}
}

 // PRIVATE (FUNCTIONS) SHHHHHHHHHHHHH

 ThreeDVector* RayTracer::calculate_diffuse(Surface* surface, vector<Light*> lights, ThreeDVector* normal, ThreeDVector* point_hit) {
 	ThreeDVector* diffuse_color = new ThreeDVector(0, 0, 0);
 	for (vector<Light*>::iterator i = lights.begin(); i != lights.end(); ++i) {
 		Light* light = *i;
 		ThreeDVector* light_direction = light->get_light_direction_from(point_hit);
 		ThreeDVector* diffuse_component = this->calculate_diffuse_helper(light, light_direction, surface->diffuse, normal);
 		diffuse_color->vector_add_bang(diffuse_component);
        delete diffuse_component;
    }
    return diffuse_color;

}

ThreeDVector* RayTracer::calculate_diffuse_helper(Light* l, ThreeDVector* light_direction, ThreeDVector* diffuse, ThreeDVector* normal) {
  float dot_product = light_direction->dot_product(normal);
  ThreeDVector light = ThreeDVector(l->red, l->green, l->blue);
  light.scalar_multiply_bang(max(dot_product, float(0)));
  return diffuse->vector_multiply(&light);
}


 ThreeDVector* RayTracer::calculate_specular(Surface* surface, vector<Light*> lights, ThreeDVector* normal, ThreeDVector* point_hit, ThreeDVector* view_direction) {
 	ThreeDVector* specular_color = new ThreeDVector(0, 0, 0);
 	for (vector<Light*>::iterator i = lights.begin(); i != lights.end(); ++i) {
 		Light* light = *i;
 		ThreeDVector* light_direction = light->get_light_direction_from(point_hit);
 		ThreeDVector* specular_component = this->calculate_specular_helper(light, light_direction, surface->specular, normal, view_direction, surface->power_coefficient);
 		specular_color->vector_add_bang(specular_component);
        delete specular_component;
    }
    return specular_color;

}

ThreeDVector* RayTracer::calculate_specular_helper(Light* l, ThreeDVector* light_direction, ThreeDVector* specular, ThreeDVector* normal, ThreeDVector* view_direction, float power_coefficient) {
	float l_dot_n = light_direction->dot_product(normal);
	ThreeDVector* two_l_dot_n_times_n = normal->scalar_multiply(2*l_dot_n);
	ThreeDVector* negative_light_direction = light_direction->scalar_multiply(-1);
	ThreeDVector* reflect = negative_light_direction->vector_add(two_l_dot_n_times_n);
	reflect->normalize_bang();
	delete two_l_dot_n_times_n;
	delete negative_light_direction;

	float r_dot_v = view_direction->dot_product(reflect);
	ThreeDVector light = ThreeDVector(l->red, l->green, l->blue);
	light.scalar_multiply_bang(pow(max(r_dot_v, float(0)), power_coefficient));
	return specular->vector_multiply(&light);
}