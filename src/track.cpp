#include "track.h"

using namespace racingai;
Track::Track() {
}

void Track::update() {
	ground_.clear();
	ground_.addVertexes(groundLine_);
	ground_.create(box2d_->getWorld());
}

void Track::setup(ofxBox2d* box) {
	box2d_ = box;
	groundLine_.addVertex(10, 10);
	groundLine_.addVertex(10, 40);
	groundLine_.addVertex(500, 600);
	groundLine_.close();
}

void racingai::Track::draw() {
	ground_.updateShape();
	ground_.draw();
}

ofxBox2dEdge Track::getGround() const {
	return ground_;
}

//vector<ofPolyline> racingai::Track::getSegments() {
//	return segments_;
//}


