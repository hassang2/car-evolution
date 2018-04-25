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

void Car::setup(ofxBox2d* box) {
	body_->setup(box->getWorld(), 200, 200, 40, 20);
	//body_->body->SetType(b2_kinematicBody);
	b2Vec2 velocity(-speed_, 0);
	body_->body->SetLinearVelocity(velocity);
}

void Car::update() {
	//position_.set(position_.x + cos(angle_ * PI / 180) * speed_, position_.y + sin(angle_ * PI / 180) * speed_);
	//body_->setPosition(position_);

	b2Vec2 velocity(cos(angle_ * PI / 180) * speed_, sin(angle_ * PI / 180) * speed_);
	body_->body->SetLinearVelocity(velocity);

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

void Car::draw() {
	ofSetColor(ofColor(150, 40, 255));
	body_->setRotation(angle_);
	body_->draw();
}

ofxBox2dRect * Car::getBody() const {
	return body_;
}

double Car::getXPos() const {
	return position_.x;
}

double Car::getYPos() const {
	return position_.y;
}

double Car::getAngle() const {
	return angle_;
}

double Car::getHeight() const {
	//return body_.height;
	return 20;
}

double Car::getWidth() const {
	//return body_.width;
	return 40;
}
