#include "raytracer.h"

RayTracer::RayTracer(vector<Surface*> _surfaces, vector<Light*> _lights) {
	surfaces = _surfaces;
	lights = _lights;
}

ThreeDVector* RayTracer::trace(Ray* ray, int depth, Surface* except_surface) {
	bool hit = false;
	Surface* first_hit;
	Record* record = new Record();
	for(vector<Surface*>::iterator it = this->surfaces.begin(); it != this->surfaces.end(); ++it) {
		Surface* surface = *it;
		if ((except_surface != NULL) && (surface == except_surface)) {
			continue;
		}
    	if (surface->hit(ray, record)) {
    		hit = true;
    		first_hit = surface;
    		ray->t_max = record->t_hit;
    	} 
	}

	if (hit) {
		ThreeDVector* point_hit = record->position_hit;
		ThreeDVector* untransformed_point_hit = record->untransformed_position_hit;
		ThreeDVector* normal = first_hit->get_normal(untransformed_point_hit, ray->direction);
		ThreeDVector* pixel_color = new ThreeDVector(0, 0, 0);

		extern ThreeDVector* global_ambient;
		ThreeDVector* view_direction = ray->direction->scalar_multiply(-1);
		view_direction->normalize_bang();
		ThreeDVector* surface_color = this->calculate_color(first_hit, this->lights, normal, point_hit, view_direction); 
		pixel_color->vector_add_bang(global_ambient);
		pixel_color->vector_add_bang(surface_color);

		if (depth > 0) {
			//REFLECTIONS. TODO USE STACK?
			ThreeDVector* normalized_ray_direction = ray->direction->normalize();
			float d_dot_n = normalized_ray_direction->dot_product(normal);
			ThreeDVector* two_d_dot_n_times_n = normal->scalar_multiply(2*d_dot_n);
			ThreeDVector* reflect_direction = normalized_ray_direction->vector_subtract(two_d_dot_n_times_n);
			Ray* reflect_ray = new Ray(point_hit->clone(), reflect_direction->clone(), 0, numeric_limits<float>::infinity());

			delete normalized_ray_direction;
			delete two_d_dot_n_times_n;
			delete reflect_direction;

			ThreeDVector* ray_color = this->trace(reflect_ray, depth - 1, first_hit);
			ThreeDVector* reflection_color = first_hit->emission->vector_multiply(ray_color);

			pixel_color->vector_add_bang(reflection_color);

			delete reflect_ray;
			delete ray_color;
			delete reflection_color;
		}

		delete point_hit;
		delete normal;
		delete view_direction;
		delete surface_color;
		
		delete record;
		return pixel_color;
	} else {
		//Return background color
		delete record;
		return new ThreeDVector(0, 0, 0);
	}
}

bool RayTracer::hits_surface(Ray* ray, Surface* except_surface) {
	Record* record = new Record();
	for(vector<Surface*>::iterator it = this->surfaces.begin(); it != this->surfaces.end(); ++it) {
		Surface* surface = *it;
		if ((except_surface != NULL) && (surface == except_surface)) {
			continue;
		}
    	if (surface->hit(ray, record)) {
    		return true;
    	} 
	}

	delete record;
	return false;
}

int RayTracer::num_hits_light(vector<Ray*> rays, Surface* except_surface) {
	int count = 0;
	for (vector<Ray*>::iterator i = rays.begin(); i != rays.end(); ++i) {
		if (!this->hits_surface(*i, except_surface)) {
			count++;
		}
	}
	return count;
}

 // PRIVATE (FUNCTIONS) SHHHHHHHHHHHHH

 ThreeDVector* RayTracer::calculate_color(Surface* surface, vector<Light*> lights, ThreeDVector* normal, ThreeDVector* point_hit, ThreeDVector* view_direction) {
 	ThreeDVector* surface_color = new ThreeDVector(0, 0, 0);
 	for (vector<Light*>::iterator i = lights.begin(); i != lights.end(); ++i) {
 		Light* light = *i;
 		ThreeDVector* light_direction = light->get_light_direction_from(point_hit);

 		int number_samples = 32;
 		vector<Ray*> shadow_rays = light->get_shadow_rays(point_hit, number_samples);
 		int num_hits_light = this->num_hits_light(shadow_rays, surface);
 		if  (num_hits_light > 0) {
 		//if (true) {
 			float multiplier = float(num_hits_light) / number_samples;

 			ThreeDVector* specular_component = this->calculate_specular_helper(light, light_direction, surface->specular, normal, view_direction, surface->power_coefficient);
 			specular_component->scalar_multiply_bang(multiplier);
	 		surface_color->vector_add_bang(specular_component);
	        delete specular_component;

 			ThreeDVector* diffuse_component = this->calculate_diffuse_helper(light, light_direction, surface->diffuse, normal);
 			diffuse_component->scalar_multiply_bang(multiplier);
 			surface_color->vector_add_bang(diffuse_component);
        	delete diffuse_component;
        } 
        
        delete light_direction;
       	shadow_rays.clear();
    }
    return surface_color;

}

ThreeDVector* RayTracer::calculate_diffuse_helper(Light* l, ThreeDVector* light_direction, ThreeDVector* diffuse, ThreeDVector* normal) {
  float dot_product = light_direction->dot_product(normal);
  ThreeDVector light = ThreeDVector(l->red, l->green, l->blue);
  light.scalar_multiply_bang(max(dot_product, float(0)));
  return diffuse->vector_multiply(&light);
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

RayTracer::~RayTracer() {
	
}