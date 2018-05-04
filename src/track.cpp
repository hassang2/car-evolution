#include "track.h"

racingai::Track::Track() {
}

void racingai::Track::setup(ofxBox2d* box, vector<shared_ptr<Car> > cars_list, Universe* world, string file_path) {
	box2d_ = box;
	world_ = world;
	cars_ = cars_list;

	for (shared_ptr<Car> car : cars_) {
		car.get()->setup(box, world_);
	}

	focus_car_ = cars_[0].get();

	loadTrack(file_path);

	setupRectangles();
}

void racingai::Track::update() {
	//pick a new focus car if current one is dead
	if (focus_car_->isDead()) {
		for (shared_ptr<Car> car : cars_) {
			if (!car.get()->isDead()) focus_car_ = car.get();
		}
	}
	calculateScores();
}

void racingai::Track::draw() {

	//centers and draws around the focus car
	center();
}

void racingai::Track::center() {
	ofPoint local_point = world_->getLocalPoint(focus_car_->getPosition().x, focus_car_->getPosition().y);
	double dx = cos(focus_car_->getAngle() * PI / 180) * focus_car_->getSpeed() - ofGetWindowWidth() / 2 + local_point.x;
	double dy = sin(focus_car_->getAngle() * PI / 180) * focus_car_->getSpeed() - ofGetWindowHeight() / 2 + local_point.y;

	if (focus_car_->isDead()) {
		dx = 0;
		dy = 0;
	}

	world_->setGlobalX(world_->getGlobalX() + dx);
	world_->setGlobalY(world_->getGlobalY() + dy);

	//centering / drawing edges
	for (shared_ptr<ofxBox2dEdge> edge : edges_) {
		ofPolyline draw_edge;

		for (ofPoint point : edge->getVertices()) {
			ofPoint local_point = world_->getLocalPoint(point.x, point.y);
			draw_edge.addVertex(local_point.x, local_point.y);
		}
		draw_edge.draw();
	}

	//centering / drawing rectangles
	ofSetColor(219, 47, 8);
	for (shared_ptr<ofxBox2dRect> rect : rectangles_) {
		ofPoint old_pos = rect->getPosition();
		double rotation = rect->getRotation();
		ofRectangle draw_rect(0, 0, rect->getWidth(), rect->getHeight());

		ofPoint local_point = world_->getLocalPoint(old_pos.x, old_pos.y);
		draw_rect.setPosition(local_point.x, local_point.y);

		ofSetRectMode(OF_RECTMODE_CENTER);
		ofPushMatrix();
		ofTranslate(draw_rect.getX(), draw_rect.getY(), 0);
		ofRotateZ(rotation);
		ofTranslate(-draw_rect.getX(), -draw_rect.getY(), 0);
		ofDrawRectangle(draw_rect);
		ofPopMatrix();
	}

	//centering score line
	ofSetColor(255, 255, 255);
	ofPolyline draw_line;
	for (ofPoint point : score_line_.getVertices()) {
		ofPoint local_point = world_->getLocalPoint(point.x, point.y);
		draw_line.addVertex(local_point.x, local_point.y);
	}

	//draw_line.draw();

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
			ofPoint global_point = world_->getGlobalPoint(edges_[i]->getVertices()[k]);
			ofs << "(" << global_point.x << ", " << global_point.y << ")" << std::endl;
		}
		ofs << "---" << std::endl;
	}

	ofs << "SCORE_LINE" << std::endl;

	for (ofPoint point : score_line_.getVertices()) {
		ofPoint global_point = world_->getGlobalPoint(point);
		ofs << "(" << global_point.x << ", " << global_point.y << ")" << std::endl;
	}

	ofs << "---";

	ofs.close();

	std::cout << "Track saved as " << file_name << std::endl;
}

void racingai::Track::loadTrack(std::string path) {
	edges_.clear();

	std::cout << "loading track" << std::endl;
	std::ifstream in_file;
	in_file.open(path);

	string line;

	if (!in_file.good()) {
		std::cout << "not a valid track" << std::endl;
		return;
	}

	bool score_line_flag = false;
	while (in_file.good()) {
		getline(in_file, line);

		if (line.find("SCORE_LINE") != string::npos) {
			score_line_flag = true;
		}
		else if (line.find("---") == string::npos) {
			int comma_index = line.find(",");
			int x = std::stoi(line.substr(1, comma_index)) - 170;
			int y = std::stoi(line.substr(comma_index + 1, line.length() - 1)) + 455;

			if (score_line_flag) {
				score_line_.addVertex(x, y);
			}
			else {
				edges_.back()->addVertex(x, y);
			}

		}
		else {
			edges_.push_back(make_shared<ofxBox2dEdge>());
		}
	}

	in_file.close();
	std::cout << "finished loading" << std::endl;
}

void racingai::Track::setupRectangles() {
	rectangles_.clear();
	for (shared_ptr<ofxBox2dEdge> edge : edges_) {

		if (edge->getVertices().size() < 2) continue;
		for (int i = 0; i < edge->getVertices().size() - 1; i++) {
			shared_ptr<ofxBox2dRect> rect = make_shared<ofxBox2dRect>();

			b2Vec2 p1(edge->getVertices()[i].x, edge->getVertices()[i].y);
			b2Vec2 p2(edge->getVertices()[i + 1].x, edge->getVertices()[i + 1].y);

			double length = b2Distance(p1, p2);
			double slope = (p2.y - p1.y) / (p2.x - p1.x);

			rect->fixture.filter.categoryBits = 0x0004;
			rect->fixture.filter.maskBits = 0x0002;
			rect->setPhysics(3.0, 0.53, 0.1);
			rect->setup(box2d_->getWorld(), (p1.x + p2.x) / 2, (p1.y + p2.y) / 2, length, 5);
			rect->setRotation(atan(slope) * 180 / PI);
			rect->body->SetType(b2_staticBody);
			rect->setData(new ContactData());

			rectangles_.push_back(rect);
		}
	}
}

void racingai::Track::addEdge() {
	if (edges_.size() > 0) 	edges_.back()->simplify();
	edges_.push_back(make_shared<ofxBox2dEdge>());
}

void racingai::Track::removeEdge() {
	//if (score_line_edit_) {
	//	score_line_.clear();
	//} else {
	//	if (edges_.size() > 0) edges_.pop_back();
	//	if (rectangles_.size() > 0) rectangles_.pop_back();
	//	std::cout << "Removed last curve" << std::endl;
	//}
}

void racingai::Track::setCars(vector <shared_ptr<Car> > new_cars) {
	cars_ = new_cars;
	focus_car_ = cars_[0].get();
}

void racingai::Track::toggleScoreLineEdit() {
	score_line_edit_ = !score_line_edit_;
	string io = score_line_edit_ ? "on" : "off";
	std::cout << "Score edit is " << io << std::endl;
}

void racingai::Track::addPoint(int x, int y) {
	if (score_line_edit_) {
		score_line_.addVertex(world_->getGlobalPoint(x, y));
		std::cout << "Added score point" << std::endl;
	}
	else {
		edges_.back()->addVertex(world_->getGlobalPoint(x, y));
		std::cout << "Added point" << std::endl;
	}
	setupRectangles();
}

void racingai::Track::removePoint() {
	//removing score point
	if (score_line_edit_) {
		if (score_line_.getVertices().size() == 0) return;
		vector<ofPoint> old_points = score_line_.getVertices();
		score_line_.clear();
		for (int i = 0; i < old_points.size() - 1; i++) {
			score_line_.addVertex(old_points[i]);
		}
		//removing track point
	}
	else {
		if (edges_.back()->getVertices().size() == 0) return;
		vector<ofPoint> old_points = edges_.back()->getVertices();
		edges_.back()->clear();
		for (int i = 0; i < old_points.size() - 1; i++) {
			edges_.back()->addVertex(old_points[i]);
		}
		rectangles_.pop_back();
	}
}

void racingai::Track::calculateScores() {
	for (shared_ptr<Car> car : cars_) {
		int closest_index = 0;
		double shortest_distance = INT_MAX;
		for (int i = 0; i < score_line_.getVertices().size(); i++) {
			double distance = score_line_[i].distance(car.get()->getPosition());
			if (distance < shortest_distance) {
				shortest_distance = distance;
				closest_index = i;
			}
		}
		car.get()->setScore(closest_index);
	}
}
