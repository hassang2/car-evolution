#include "car.h"
#include <cmath>

//using namespace racingai;

ofVec2f racingai::Car::getBodySize() const {
	return body_size_;
};

racingai::Car::Car() {
	body_ = new ofxBox2dRect();
	body_->setPhysics(3.0, 0.53, 0.1);
}

void racingai::Car::setup(ofxBox2d* box) {
	box2d_ = box;
	body_->setup(box2d_->getWorld(), 0, 0, 40, 20);
	initSensors();
}

void racingai::Car::setSpeed(double s) {
	speed_ = s;
}

void racingai::Car::update() {
	body_->setPosition(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);
	updateSensorValues();
}

void racingai::Car::draw() {
	ofSetColor(ofColor(150, 40, 255));
	body_->setRotation(angle_);
	body_->draw();
	drawSensors();
}

void racingai::Car::updateSensorValues() {
	initSensors();
	//RayCaster call_back;

	for (Sensor sensor : sensors_) {
		RayCaster call_back;

		b2Vec2 p1 = sensor.input.p1;
		b2Vec2 p2 = sensor.input.p2;
		p1.x /= box2d_->scale;
		p1.y /= box2d_->scale;
		p2.x /= box2d_->scale;
		p2.y /= box2d_->scale;

		box2d_->getWorld()->RayCast(&call_back, p1, p2);

		if (call_back.didCollide()) {
			b2Vec2 intersection = call_back.getIntersectionPoint();
			sensor.value = ofDist(sensor.input.p1.x, sensor.input.p1.y, intersection.x * box2d_->scale, intersection.y * box2d_->scale);
			std::cout << sensor.value << std::endl;
		}
	}
}

void racingai::Car::drawSensors() {
	ofSetColor(66, 244, 212);
	for (Sensor sensor : sensors_) {
		sensor.line = make_shared<ofPolyline>();
		sensor.line.get()->clear();
		sensor.line.get()->addVertex(sensor.input.p1.x, sensor.input.p1.y);
		sensor.line.get()->addVertex(sensor.input.p2.x, sensor.input.p2.y);
		sensor.line.get()->draw();
	}
}

void racingai::Car::initSensors() {
	sensors_.clear();

	//middle
	Sensor middle_sensor;
	middle_sensor.length = 120;

	double x_m = ofGetWindowWidth() / 2 + WIDTH / 2 * cos(angle_ * PI / 180);
	double y_m = ofGetWindowHeight() / 2 + WIDTH / 2 * sin(angle_ * PI / 180);
	b2Vec2 p1_m(x_m, y_m);
	b2Vec2 p2_m = p1_m + middle_sensor.length * b2Vec2(cos(angle_ * PI / 180), sin(angle_ * PI / 180));

	middle_sensor.input.p1 = p1_m;
	middle_sensor.input.p2 = p2_m;
	middle_sensor.input.maxFraction = 1.0;
	sensors_.push_back(middle_sensor);

	
	//Left
	Sensor left_sensor;
	left_sensor.length = 120;

	double x_l = ofGetWindowWidth() / 2 + WIDTH / 2 * cos(angle_ * PI / 180) + HEIGHT / 2 * sin(angle_ * PI / 180);
	double y_l = ofGetWindowHeight() / 2 + WIDTH / 2 * sin(angle_ * PI / 180) - HEIGHT / 2 * cos(angle_ * PI / 180);
	b2Vec2 p1_l(x_l, y_l);
	b2Vec2 p2_l = p1_l + left_sensor.length * b2Vec2(cos((angle_ - 25) * PI / 180), sin((angle_ - 25) * PI / 180));

	left_sensor.input.p1 = p1_l;
	left_sensor.input.p2 = p2_l;
	left_sensor.input.maxFraction = 1.0;
	sensors_.push_back(left_sensor);

	//Right
	Sensor right_sensor;
	right_sensor.length = 120;

	double x_r = ofGetWindowWidth() / 2 + WIDTH / 2 * cos(angle_ * PI / 180) - HEIGHT / 2 * sin(angle_ * PI / 180);
	double y_r = ofGetWindowHeight() / 2 + WIDTH / 2 * sin(angle_ * PI / 180) + HEIGHT / 2 * cos(angle_ * PI / 180);
	b2Vec2 p1_r(x_r, y_r);
	b2Vec2 p2_r = p1_r + right_sensor.length * b2Vec2(cos((angle_ + 25) * PI / 180), sin((angle_ + 25) * PI / 180));
	
	right_sensor.input.p1 = p1_r;
	right_sensor.input.p2 = p2_r;
	right_sensor.input.maxFraction = 1.0;
	sensors_.push_back(right_sensor);
	
}

int racingai::Car::getScore() const {
	return score_;
}

void racingai::Car::swerveRight() {
	angle_ -= SWERVE_AMOUNT;
	if (angle_ < 0) angle_ += 360;
}

void racingai::Car::swerveLeft() {
	angle_ = fmod(angle_ + SWERVE_AMOUNT, 360);
}

ofxBox2dRect * racingai::Car::getBody() const {
	return body_;
}

double racingai::Car::getXPos() const {
	return body_->getPosition().x;
}

double racingai::Car::getYPos() const {
	return body_->getPosition().y;
}

double racingai::Car::getAngle() const {
	return angle_;
}

double racingai::Car::getSpeed() const {
	return speed_;
}

double racingai::Car::getHeight() const {
	return HEIGHT;
}

double racingai::Car::getWidth() const {
	return WIDTH;
}


vector<racingai::Sensor> racingai::Car::getSensors() const{
	return sensors_;
}

float32 racingai::RayCaster::ReportFixture(b2Fixture * fixture, const b2Vec2 & point, const b2Vec2 & normal, float32 fraction) {
	collide = true;
	intersection_point_ = point;
	return fraction;
}

b2Vec2 racingai::RayCaster::getIntersectionPoint() const {
	return intersection_point_;
}

bool racingai::RayCaster::didCollide() const {
	return collide;
}
