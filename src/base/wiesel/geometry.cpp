#include "geometry.h"

using namespace wiesel;


dimension::dimension() 
: width(0.0f), height(0.0f) {
	return;
}

dimension::dimension(float width, float height) 
: width(width), height(height) {
	return;
}

dimension::dimension(const dimension &other) 
: width(other.width), height(other.height) {
	return;
}


void dimension::scale(float s) {
	this->width  *= s;
	this->height *= s;
}

void dimension::scale(float sx, float sy) {
	this->width  *= sx;
	this->height *= sy;
}
