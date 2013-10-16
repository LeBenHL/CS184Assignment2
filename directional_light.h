#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include "light.h"
#include "three_d_vector.h"

class DirectionalLight: public Light {
  public:
  	ThreeDVector* direction;
  	ThreeDVector* get_light_direction_from(ThreeDVector* postion);
  	Ray* get_shadow_ray(ThreeDVector* postion);
  	vector<Ray*> get_shadow_rays(ThreeDVector* position, int sample_size);
  	void apply_transformation(Eigen::Matrix4f transformation);
  	double long get_attenuation_factor(ThreeDVector* position);
  	DirectionalLight(long double, long double, long double, long double, long double, long double);
  	~DirectionalLight();
};

#endif