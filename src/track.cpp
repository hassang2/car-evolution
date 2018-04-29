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

	for (int i = 0; i < edges_.size(); i++) {
		for (int k = 0; k < edges_[i]->getVertices().size(); k++) {
			ofPoint global_point = getGlobalPoint(edges_[i]->getVertices()[k]);
			ofs << "(" << global_point.x << ", " << global_point.y << ")" << std::endl;
		}
		ofs << "---" << std::endl;
	}
	
	ofs.close();

	std::cout << "Track saved as " << file_name << std::endl;
}

void racingai::Track::loadTrack(std::string path) {
	std::cout << "loading track" << std::endl;
	std::ifstream in_file;
	in_file.open(path);

	string line;

	if (!in_file.good()) {
		std::cout << "not a valid track" << std::endl;
		return;
	}
	while (in_file.good()) {
		getline(in_file, line);
		if (line.find("---") == string::npos) {
			std::cout << line << std::endl;
			int comma_index = line.find(",");
			int x = std::stoi(line.substr(1, comma_index));
			int y = std::stoi(line.substr(comma_index + 1, line.length() - 1));

			edges_.back().get()->addVertex(x, y);
		} else {
			edges_.push_back(make_shared<ofxBox2dEdge>());
		}
	}

	in_file.close();
	std::cout << "finished loading" << std::endl;
}

void Track::update() {
	center();

	//creating the edges_
	for (std::shared_ptr<ofxBox2dEdge> edge : edges_) {
		edge.get()->create(box2d_->getWorld());
	}
}

void Track::center() {
	for (std::shared_ptr<ofxBox2dEdge> edge : edges_) {
		vector<ofPoint> old_points = edge.get()->getVertices();
		edge.get()->clear();
		
		double dx = cos(game_car_->getAngle() * PI / 180) * game_car_->getSpeed() + ofGetWindowWidth() / 2 - game_car_->getXPos();
		double dy = sin(game_car_->getAngle() * PI / 180) * game_car_->getSpeed() + ofGetWindowHeight() / 2 - game_car_->getYPos();

		global_x_ += dx;
		global_y_ += dy;
		for (ofPoint point : old_points) {
			edge.get()->addVertex(point.x + dx, point.y + dy);
		}
	}
}


void racingai::Track::addEdge() {
	if (edges_.size() > 0) 	edges_.back()->simplify();
	edges_.push_back(make_shared<ofxBox2dEdge>());
}

void racingai::Track::removeEdge() {
	if (edges_.size() > 0) edges_.pop_back();
	std::cout << "Removed last curve" << std::endl;
}

void racingai::Track::draw() {

	//for (std::shared_ptr<ofxBox2dEdge> edge : edges_) {
	//	edge->updateShape();
	//	edge->draw();
	//}
	for (int i = 0; i<edges_.size(); i++) {
		edges_[i].get()->draw();
	}
}

void racingai::Track::addPoint(int x, int y) {
	edges_.back().get()->addVertex(x, y);
	std::cout << "Added point" << std::endl;
	update();
}

void racingai::Track::removePoint() {
	if (edges_.back().get()->getVertices().size == 0) return;
	vector<ofPoint> old_points = edges_.back().get()->getVertices();
	edges_.back().get()->clear();
	for (int i = 0; i < old_points.size() - 1; i++) {
		edges_.back().get()->addVertex(old_points[i]);
	}
}

ofPoint racingai::Track::getLocalPoint(double x, double y) const {
	return ofPoint(x, y);
}

ofPoint racingai::Track::getLocalPoint(ofPoint point) const {
	return getLocalPoint(point.x, point.y);
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


