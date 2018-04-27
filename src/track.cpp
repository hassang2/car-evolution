#include "track.h"

using namespace racingai;
Track::Track() {
}

void Track::setup(ofxBox2d* box, Car* car, string file_path) {
	box2d_ = box;
	game_car_ = car;
	loadTrack(file_path);
}



void racingai::Track::saveTrack() const {

	//find an unused index for the track name
	int file_index = 1;
	string file_name = "track" + ofToString(file_index);
	ifstream f("../tracks/" + file_name);
	while (ifstream("../tracks/" + file_name).good()) {
		file_index++;
		file_name = "track" + ofToString(file_index);
	}

	std::ofstream ofs("../tracks/" + file_name, std::ios::out);
	
	for (int i = 0; i < groundLine_.getVertices().size() - 1; i++) {
		ofPoint global_point = getGlobalPoint(groundLine_.getVertices()[i]);
		ofs << "(" << global_point.x << ", " << global_point.y << ")" << std::endl;
	}

	//to prevent a new line character at the end of the file
	ofPoint global_point = getGlobalPoint(groundLine_.getVertices()[groundLine_.getVertices().size() - 1]);
	ofs << "(" << global_point.x << ", " << global_point.y << ")";

	ofs.close();

	std::cout << "Track saved as " << file_name << std::endl;
}

void racingai::Track::loadTrack(std::string path) {
	std::cout << "loading track" << std::endl;
	std::ifstream inFile;
	inFile.open(path);

	string line;

	if (!inFile.good()) {
		std::cout << "not a valid track" << std::endl;
		return;
	}
	while (inFile.good()) {
		getline(inFile, line);
		int comma_index = line.find(",");
		int x = std::stoi(line.substr(1, comma_index));
		int y = std::stoi(line.substr(comma_index + 1, line.length() - 1));

		ofPoint new_point = getLocalPoint(ofPoint(x, y));
		vertices_.push_back(new_point);
	}

	//vertices_.insert(vertices_.begin(), vertices_.front());
	//vertices_.push_back(vertices_.back());

	groundLine_.addVertices(vertices_);

	//scaling the size
	//vector<ofPoint> old_points = groundLine_.getVertices();
	//groundLine_ = ofPolyline();

	//for (ofPoint &point : vertices_) {
	//	point = ofPoint(ofMap(point.x, 0, 500, 0, 5000) - 10 * ofGetWindowWidth() / 2,  ofMap(point.y, 0, 500, 0 , 5000) - 10 * ofGetWindowWidth() / 2);
	//	groundLine_.curveTo(point);
	//}

	//vertices_.push_back(vertices_.back());
	//groundLine_.addVertex(vertices_.back());
	inFile.close();
	std::cout << "finished loading" << std::endl;

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

	update();
	std::cout << "Removed last point" << std::endl;
}

ofPoint racingai::Track::getLocalPoint(double x, double y) const {
	return ofPoint(x, y);
}

ofPoint racingai::Track::getLocalPoint(ofPoint point) const {
	return getLocalPoint(point.x, point.y);
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

ofPoint racingai::Track::getGlobalPoint(double x, double y) const {
	ofPoint global_point(x + global_x_, y + global_y_);
	return global_point;
}

ofPoint racingai::Track::getGlobalPoint(ofPoint point) const {
	return getGlobalPoint(point.x, point.y);
}

//vector<ofPolyline> racingai::Track::getSegments() {
//	return segments_;
//}


