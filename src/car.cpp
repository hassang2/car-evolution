#include "car.h"
#include <cmath>

using namespace racingai;

ofVec2f Car::getBodySize() const {
	return body_size_;
};

Car::Car() {
	position_.set(100, 100);
	body_ = new ofxBox2dRect();
	body_->setPhysics(3.0, 0.53, 0.1);
}

void Car::update() {
	position_.set(position_.x + cos(angle_ * PI / 180) * speed_, position_.y + sin(angle_ * PI / 180) * speed_);
	body_->setPosition(position_);
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
	return position_.x;
}

double racingai::Car::getYPos() const {
	return position_.y;
}

double racingai::Car::getAngle() const {
	return angle_;
}

double racingai::Car::getHeight() const {
	//return body_.height;
	return 20;
}

double racingai::Car::getWidth() const {
	//return body_.width;
	return 40;
}
