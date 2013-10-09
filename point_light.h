#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "light.h"
#include "three_d_vector.h"

class PointLight: public Light {
  public:
  	ThreeDVector* position;
  	PointLight(float, float, float, float, float, float);
  	~PointLight();
  	ThreeDVector* get_light_direction_from(ThreeDVector* postion);
  	vector<ThreeDVector*> get_light_samples(ThreeDVector* position);
  	Ray* get_shadow_ray(ThreeDVector* postion);
};

#endif