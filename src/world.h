#pragma once
#include "ofMain.h"

namespace racingai {

	class Universe {
	private:
		double global_x_ = -ofGetWindowWidth() / 2;
		double global_y_ = -ofGetWindowHeight() / 2;

	public:
		double getGlobalX() { return global_x_; }
		double getGlobalY() { return global_y_; }
		void setGlobalX(double x);
		void setGlobalY(double y);
		//converts window coordinates to global coordinates
		ofPoint getGlobalPoint(double x, double y) const;
		ofPoint getGlobalPoint(ofPoint point) const;
		//converts global coordinates to window coordinates
		ofPoint getLocalPoint(double x, double y);
		ofPoint getLocalPoint(ofPoint point);
	};
}