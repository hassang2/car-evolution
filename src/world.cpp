#include "world.h"

ofPoint racingai::Universe::getLocalPoint(double x, double y) {
	return ofPoint(x - global_x_, y - global_y_);
}

ofPoint racingai::Universe::getLocalPoint(ofPoint point) {
	return getLocalPoint(point.x, point.y);
}

void racingai::Universe::setGlobalX(double x) {
	global_x_ = x;
}

void racingai::Universe::setGlobalY(double y) {
	global_y_ = y;
}

ofPoint racingai::Universe::getGlobalPoint(double x, double y) {
	return ofPoint(x + global_x_, y + global_y_);
}

ofPoint racingai::Universe::getGlobalPoint(ofPoint point) {
	return getGlobalPoint(point.x, point.y);
}