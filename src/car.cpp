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
	body_->setup(box->getWorld(), -500, -500, 40, 20);
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
	//return body_.height;
	return 20;
}

double Car::getWidth() const {
	//return body_.width;
	return 40;
}
