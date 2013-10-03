#include <cmath>
#include "three_d_vector.h"
#include <iostream>
using namespace std;

ThreeDVector::ThreeDVector(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}

float ThreeDVector::magnitude(){
	float x = this->x;
	float y = this->y;
	float z = this->z;
	return sqrt(x*x+y*y+z*z);
}

ThreeDVector* ThreeDVector::normalize(){
	//cout << "BEFORE: " << this->x << ", " << this->y << ", " << this->z << endl;
	float mag = magnitude();
	return new ThreeDVector(this->x / mag, this->y / mag, this->z / mag);
	//cout << "AFTER: " << this->x << ", " << this->y << ", " << this->z << endl;
}


float ThreeDVector::dot_product(ThreeDVector* v){
	return this->x * v->x + this->y * v->y + this->z * v->z;
}


ThreeDVector* ThreeDVector::scalar_multiply(float k){	
	return new ThreeDVector(this->x * k, this->y * k, this->z * k);
}

void ThreeDVector::vector_add(ThreeDVector* v){
	this->x += v->x;
	this->y += v->y;
	this->z += v->z;
}

ThreeDVector* ThreeDVector::vector_subtract(ThreeDVector* v){
	return new ThreeDVector(this->x - v->x, this->y - v->y, this->z - v->z);
}

ThreeDVector* ThreeDVector::vector_multiply(ThreeDVector* v){
	return new ThreeDVector(v->x * this->x, v->y * this->y, v->z * this->z);
}

ThreeDVector* ThreeDVector::cross_product(ThreeDVector* v){
	return new ThreeDVector(this->y * v->z - this->z * v->y, this->z * v->x - this->x * v->z, this->x * v->y - this->y * v->x);                                                                         
}

ThreeDVector* ThreeDVector::clone(){
	return new ThreeDVector(this->x, this->y, this->z);
}

char* ThreeDVector::repr() {
	char buffer[500];
	sprintf(buffer, "<ThreeDVector, x = %0.2f, y = %0.2f, z = %0.2f>", this->x, this->y, this->z);
	return buffer;
}