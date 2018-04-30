#include "car.h"
#include <cmath>

using namespace racingai;

ofVec2f Car::getBodySize() const {
	return body_size_;
};

Car::Car() {
	body_ = new ofxBox2dRect();
	body_->setPhysics(3.0, 0.53, 0.1);
}

void Car::setup(ofxBox2d* box) {
	body_->setup(box->getWorld(), 0, 0, 40, 20);
	//initSensors();
}

void racingai::Car::setSpeed(double s) {
	speed_ = s;
}

void Car::update() {
	body_->setPosition(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);
}

void Car::draw() {
	ofSetColor(ofColor(150, 40, 255));
	body_->setRotation(angle_);
	body_->draw();
}

bool Car::isDead() const {
	// Car is dead if the head is off screen
	//if (head_->position.x < 0
	//	|| head_->position.y < 0
	//	|| head_->position.x > screen_dims_.x - body_size_.x
	//	|| head_->position.y > screen_dims_.y - body_size_.y) {
	//	return true;
	//}

	// Car is not dead yet :D
	return false;
}

void Car::initSensors() {
	//middle
	Sensor middle_sensor;
	middle_sensor.length = 8;

	double x_m = ofGetWindowWidth() / 2 + WIDTH / 2 * cos(angle_);
	double y_m = ofGetWindowHeight() / 2 + WIDTH / 2 * sin(angle_);
	b2Vec2 p1_m(x_m, y_m);
	b2Vec2 p2_m = p1_m + middle_sensor.length * b2Vec2(sin(angle_), cos(angle_));

	middle_sensor.input.p1 = p1_m;
	middle_sensor.input.p2 = p2_m;
	middle_sensor.input.maxFraction = 1.0;
	sensors_.push_back(middle_sensor);

	//Left
	Sensor left_sensor;
	left_sensor.length = 10;

	double x_l = ofGetWindowWidth() / 2 + WIDTH / 2 * cos(angle_) - HEIGHT / 2 * sin(angle_);
	double y_l = ofGetWindowHeight() / 2 + WIDTH / 2 * sin(angle_) - HEIGHT / 2 * cos(angle_);
	b2Vec2 p1_l(x_l, y_l);
	b2Vec2 p2_l = p1_l + left_sensor.length * b2Vec2(sin(angle_ + 15), cos(angle_ + 15));

	left_sensor.input.p1 = p1_l;
	left_sensor.input.p2 = p2_l;
	left_sensor.input.maxFraction = 1.0;
	sensors_.push_back(left_sensor);

	//Right
	Sensor right_sensor;
	right_sensor.length = 10;

	double x_r = ofGetWindowWidth() / 2 + WIDTH / 2 * cos(angle_) + HEIGHT / 2 * sin(angle_);
	double y_r = ofGetWindowHeight() / 2 + WIDTH / 2 * sin(angle_) + HEIGHT / 2 * cos(angle_);
	b2Vec2 p1_r(x_r, y_r);
	b2Vec2 p2_r = p1_r + right_sensor.length * b2Vec2(sin(angle_ - 15), cos(angle_ - 15));
	
	right_sensor.input.p1 = p1_r;
	right_sensor.input.p2 = p2_r;
	right_sensor.input.maxFraction = 1.0;
	sensors_.push_back(right_sensor);
}

int Car::getScore() const {
	return score_;
}

void Car::swerveRight() {
	angle_ -= SWERVE_AMOUNT;
	if (angle_ < 0) angle_ += 360;
}

void Car::swerveLeft() {
	angle_ = fmod(angle_ + SWERVE_AMOUNT, 360);
}

ofxBox2dRect * Car::getBody() const {
	return body_;
}

double Car::getXPos() const {
	return body_->getPosition().x;
}

double Car::getYPos() const {
	return body_->getPosition().y;
}

double Car::getAngle() const {
	return angle_;
}

double racingai::Car::getSpeed() const {
	return speed_;
}

double Car::getHeight() const {
	return HEIGHT;
}

double Car::getWidth() const {
	return WIDTH;
}
