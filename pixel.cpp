#include "pixel.h"

Pixel::Pixel(int _x, int _y, Camera* _camera) {
	x = _x;
	y = _y;
	camera = _camera;
}

Pixel::~Pixel() {
}

ThreeDVector* Pixel::get_color() {
	return new ThreeDVector(255, 0, 0);
}