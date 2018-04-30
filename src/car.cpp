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
}

void racingai::Car::draw() {
	ofSetColor(ofColor(150, 40, 255));
	body_->setRotation(angle_);
	body_->draw();
}

bool racingai::Car::isDead() const {
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

void racingai::Car::updateSensorValues() {


	for (Sensor& sensor : sensors_) {
		double closestFraction = 1;
		b2Vec2 intersectionNormal(0, 0);
		//input = sensor.input;
		for (b2Body* b = box2d_->getWorld()->GetBodyList(); b; b = b->GetNext()) {
			for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) {
				if (!f->RayCast(&sensor.output, sensor.input, 0)) continue;
				if (sensor.output.fraction < closestFraction) {
					closestFraction = sensor.output.fraction;
					intersectionNormal = sensor.output.normal;
				}
			}
		}
		b2Vec2 intersectionPoint = sensor.input.p1 + closestFraction * (sensor.input.p2 - sensor.input.p1);
		sensor.value = ofDist(intersectionPoint.x, intersectionPoint.y, sensor.input.p1.x, sensor.input.p1.y);
	}
}

void racingai::Car::initSensors() {
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
