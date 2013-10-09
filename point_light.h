#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "light.h"
#include "three_d_vector.h"

class PointLight: public Light {
  public:
  	ThreeDVector* position;
  	float radius;
  	PointLight(float, float, float, float, float, float);
  	~PointLight();
  	ThreeDVector* get_light_direction_from(ThreeDVector* postion);
  	vector<Ray*> get_shadow_rays(ThreeDVector* position, int sample_size);
  	Ray* get_shadow_ray(ThreeDVector* postion);
};

#endif