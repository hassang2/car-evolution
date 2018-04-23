#include "track.h"

racingai::Track::Track() {
	setup();
}

void racingai::Track::setup() {
	ofPolyline a;
	a.curveTo(350, 100);  // These curves are Catmull-Rom splines
	a.curveTo(350, 100);  // Necessary Duplicate for Control Point
	a.curveTo(400, 150);
	a.curveTo(450, 100);
	a.curveTo(500, 150);
	a.curveTo(550, 100);
	a.curveTo(550, 100);

	segments_.push_back(a);
}

vector<ofPolyline> racingai::Track::getSegments() {
	return segments_;
}


