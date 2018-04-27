#include "track.h"

using namespace racingai;
Track::Track() {
}

void Track::setup(ofxBox2d* box, Car* car) {
	box2d_ = box;
	game_car_ = car;

	//designing the track

	//vertices_.push_back(ofPoint(237, 409));  // These curves are Catmull-Rom splines
	//vertices_.push_back(ofPoint(237, 409));  //Necessary Duplicate for Control Point
	//vertices_.push_back(ofPoint(270, 415)); 
	//vertices_.push_back(ofPoint(295, 423));
	//vertices_.push_back(ofPoint(321, 431));
	//vertices_.push_back(ofPoint(357, 440));
	//vertices_.push_back(ofPoint(383, 439));
	//vertices_.push_back(ofPoint(406, 431));
	//vertices_.push_back(ofPoint(424, 418));
	//vertices_.push_back(ofPoint(428, 409));
	//vertices_.push_back(ofPoint(433, 402));
	//vertices_.push_back(ofPoint(447, 397));
	//vertices_.push_back(ofPoint(455, 388));
	//vertices_.push_back(ofPoint(451, 372));
	//vertices_.push_back(ofPoint(429, 358));
	//vertices_.push_back(ofPoint(390, 346));
	//vertices_.push_back(ofPoint(325, 343));
	//vertices_.push_back(ofPoint(260, 337));
	//vertices_.push_back(ofPoint(230, 327));
	//vertices_.push_back(ofPoint(230, 327));

	vertices_.push_back(ofPoint(243, 407));
	vertices_.push_back(ofPoint(243, 407));
	vertices_.push_back(ofPoint(251, 414));
	vertices_.push_back(ofPoint(258, 426));
	vertices_.push_back(ofPoint(279, 444));
	vertices_.push_back(ofPoint(291, 438));
	vertices_.push_back(ofPoint(302, 436));
	vertices_.push_back(ofPoint(311, 429));
	vertices_.push_back(ofPoint(319, 438));
	vertices_.push_back(ofPoint(332, 445));
	vertices_.push_back(ofPoint(349, 430));
	vertices_.push_back(ofPoint(361, 411));
	vertices_.push_back(ofPoint(396, 433));
	vertices_.push_back(ofPoint(407, 401));
	vertices_.push_back(ofPoint(425, 392));
	vertices_.push_back(ofPoint(445, 398));
	vertices_.push_back(ofPoint(455, 387));
	vertices_.push_back(ofPoint(448, 337));
	vertices_.push_back(ofPoint(427, 320));
	vertices_.push_back(ofPoint(410, 350));
	vertices_.push_back(ofPoint(401, 374));
	vertices_.push_back(ofPoint(387, 380));
	vertices_.push_back(ofPoint(375, 375));
	vertices_.push_back(ofPoint(371, 322));
	vertices_.push_back(ofPoint(357, 314));
	vertices_.push_back(ofPoint(345, 320));
	vertices_.push_back(ofPoint(332, 373));
	vertices_.push_back(ofPoint(326, 379));
	vertices_.push_back(ofPoint(316, 376));
	vertices_.push_back(ofPoint(310, 363));
	vertices_.push_back(ofPoint(312, 335));
	vertices_.push_back(ofPoint(300, 332));
	vertices_.push_back(ofPoint(298, 358));
	vertices_.push_back(ofPoint(295, 361));
	vertices_.push_back(ofPoint(289, 362));
	vertices_.push_back(ofPoint(275, 361));
	vertices_.push_back(ofPoint(248, 344));
	vertices_.push_back(ofPoint(235, 345));
	vertices_.push_back(ofPoint(232, 353));
	vertices_.push_back(ofPoint(244, 365));
	vertices_.push_back(ofPoint(235, 387));
	vertices_.push_back(ofPoint(215, 382));
	vertices_.push_back(ofPoint(211, 390));
	vertices_.push_back(ofPoint(218, 397));
	vertices_.push_back(ofPoint(217, 406));
	vertices_.push_back(ofPoint(206, 423));
	vertices_.push_back(ofPoint(230, 414));
	vertices_.push_back(ofPoint(243, 407));
	vertices_.push_back(ofPoint(243, 407));

	groundLine_.addVertices(vertices_);

	vector<ofPoint> old_points = groundLine_.getVertices();
	groundLine_ = ofPolyline();

	for (ofPoint &point : vertices_) {
		point = ofPoint(ofMap(point.x, 0, 500, 0, 5000) - 10 * ofGetWindowWidth() / 2,  ofMap(point.y, 0, 500, 0 , 5000) - 10 * ofGetWindowWidth() / 2);
		groundLine_.curveTo(point);
	}

	//groundLine_.addVertices(vertices_);

	//groundLine_.addVertex(2000, 400);
}

void racingai::Track::loadTrack(std::string path) {
	std::ifstream inFile;
	inFile.open(path);

	string line;

	while (inFile.good()) {
		getline(inFile, line);
		int comma_index = line.find(",");
		int x = std::stoi(line.substr(1, comma_index));
		int y = std::stoi(line.substr(comma_index + 1, line.length() - 1));

		ofPoint new_point = ofPoint(x - 1660, y - 260);
		vertices_.push_back(new_point);
		groundLine_.addVertex(new_point);
	}

	vertices_.push_back(vertices_.back());
	groundLine_.addVertex(vertices_.back());
	inFile.close();
}

void racingai::Track::saveTrack() const {
	int file_index = 1;
	string file_name = "track" + ofToString(file_index);
	ifstream f(file_name);
	while (ifstream(file_name).good()) {
		file_index++;
		file_name = "track" + ofToString(file_index);
	}

	std::ofstream ofs(file_name, std::ios::out);
	
	for (ofPoint point : groundLine_.getVertices()) {
		ofs << "(" << point.x << ", " << point.y << ")" << std::endl;
	}
	ofs.close();

	std::cout << "Track saved as " << file_name << std::endl;
}

void Track::update() {
	center();

	//creating the edges
	ground_.clear();
	ground_.addVertexes(groundLine_);
	ground_.create(box2d_->getWorld());
}

void Track::center() {
	vector<ofPoint> old_points = groundLine_.getVertices();
	groundLine_ = ofPolyline();

	double dx = cos(game_car_->getAngle() * PI / 180) * game_car_->getSpeed() + ofGetWindowWidth() / 2 - game_car_->getXPos();
	double dy = sin(game_car_->getAngle() * PI / 180) * game_car_->getSpeed() + ofGetWindowHeight() / 2 - game_car_->getYPos();
	global_x_ += dx;
	global_y_ += dy;
	for (ofPoint point : old_points) {
		groundLine_.addVertex(point.x + dx, point.y + dy);
	}
}

void racingai::Track::draw() {
	ground_.updateShape();
	ground_.draw();
}

void racingai::Track::addPoint(int x, int y) {
	ofPoint new_point = ofPoint(x, y);
	vertices_.push_back(new_point);
	vertices_.push_back(new_point);
	groundLine_.addVertex(new_point);
	groundLine_.addVertex(new_point);

	added_vertices_.push_back(ofPoint(x, y));
	update();
	std::cout << "Added point" << std::endl;
}

void racingai::Track::removePoint() {
	vector<ofPoint> old_points = groundLine_.getVertices();
	groundLine_ = ofPolyline();
	old_points.pop_back();
	old_points.pop_back();

	ofPoint back = vertices_.back();

	for (ofPoint point : old_points) {
		groundLine_.addVertex(point.x, point.y);
	}

	added_vertices_.pop_back();

	update();
	std::cout << "Removed last point" << std::endl;
}

void racingai::Track::printPaths() const {
	for (ofPoint point : added_vertices_) {
		std::cout << "(" + ofToString(point.x) + ", " + ofToString(point.y) + ")" << std::endl;
	}
}


ofPoint racingai::Track::getLocalPoint(double x, double y) {
	//TODO
	return ofPoint();
}

ofxBox2dEdge Track::getGround() const {
	return ground_;
}

//int racingai::Track::getGlobalX() const {
//	return global_x_;
//}
//
//int racingai::Track::getGlobalY() const {
//	return global_y_;
//}

void racingai::Track::setGlobalX(double x) {
	global_x_ = x;
}

void racingai::Track::setGlobalY(double y) {
	global_y_ = y;
}

ofPoint racingai::Track::getGlobalPoint(double x, double y) {
	ofPoint global_point(x + global_x_, y + global_y_);
	return global_point;
}

//vector<ofPolyline> racingai::Track::getSegments() {
//	return segments_;
//}


