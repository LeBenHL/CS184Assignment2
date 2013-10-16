#include <cmath>
#include "three_d_vector.h"
#include <iostream>
using namespace std;

ThreeDVector::ThreeDVector(long double _x, long double _y, long double _z) {
	x = _x;
	y = _y;
	z = _z;
}

long double ThreeDVector::magnitude(){
	long double x = this->x;
	long double y = this->y;
	long double z = this->z;
	return sqrt(x*x+y*y+z*z);
}

ThreeDVector* ThreeDVector::normalize(){
	//cout << "BEFORE: " << this->x << ", " << this->y << ", " << this->z << endl;
	long double mag = magnitude();
	return new ThreeDVector(this->x / mag, this->y / mag, this->z / mag);
	//cout << "AFTER: " << this->x << ", " << this->y << ", " << this->z << endl;
}

void ThreeDVector::normalize_bang(){
	//cout << "BEFORE: " << this->x << ", " << this->y << ", " << this->z << endl;
	long double mag = magnitude();
	this->x /= mag;
	this->y /= mag;
	this->z /= mag;
	//cout << "AFTER: " << this->x << ", " << this->y << ", " << this->z << endl;
}



long double ThreeDVector::dot_product(ThreeDVector* v){
	return this->x * v->x + this->y * v->y + this->z * v->z;
}


ThreeDVector* ThreeDVector::scalar_multiply(long double k){	
	return new ThreeDVector(this->x * k, this->y * k, this->z * k);
}

void ThreeDVector::scalar_multiply_bang(long double k){	
	this->x *= k;
	this->y *= k;
	this->z *= k;
}

ThreeDVector* ThreeDVector::vector_add(ThreeDVector* v){
	return new ThreeDVector(this->x + v->x, this->y + v->y, this->z + v->z);
}

void ThreeDVector::vector_add_bang(ThreeDVector* v){
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
	char* buffer = new char[1000];
	sprintf(buffer, "<ThreeDVector, x = %0.2Lf, y = %0.2Lf, z = %0.2Lf>", this->x, this->y, this->z);
	return buffer;
}