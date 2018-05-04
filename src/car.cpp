#include "car.h"
#include <cmath>


racingai::Car::Car() {
	body_ = ofxBox2dRect();
	body_.setPhysics(3.0, 0.53, 0.1);
	body_.fixture.filter.categoryBits = 0x0002;
	body_.fixture.filter.maskBits = 0x0004;
}

//racingai::Car::Car(const Car& source) {
//	dead_ = source.dead_;
//	angle_ = source.angle_;
//	speed_ = source.speed_;
//	//body_ = new ofxBox2dRect(source.body_);
//	
//	box2d_ = source.box2d_;
//	world_ = source.world_;
//
//}

racingai::Car::~Car() {
}

//racingai::Car & racingai::Car::operator=(const Car & source) {
//	// TODO: insert return statement here
//}

void racingai::Car::setup(ofxBox2d* box, Universe* world) {
	box2d_ = box;
	world_ = world;

	ofAddListener(box2d_->contactStartEvents, this, &Car::contactStart);
	ofAddListener(box2d_->contactEndEvents, this, &Car::contactEnd);

	position_ = ofPoint(0, 0);
	body_.setup(box2d_->getWorld(), 0, 0, 40, 20);
	body_.setVelocity(cos(angle_ * PI / 180) * speed_, sin(angle_ * PI / 180) * speed_);
	body_.setData(new ContactData());

	initSensors();
}

void racingai::Car::update() {
	position_ = body_.getPosition();
	body_.setVelocity(cos(angle_ * PI / 180) * speed_, sin(angle_ * PI / 180) * speed_);
	body_.setRotation(angle_);

	ContactData * data = (ContactData*)body_.getData();
	if (data && data->isInContact()) {
		dead_ = true;
		body_.setVelocity(0, 0);
	}

	//updates the position
	updateSensors();

	//updates the value
	updateSensorValues();
}

void racingai::Car::draw() {

	ofPoint local_point = world_->getLocalPoint(position_);

	ofRectangle draw_rect(local_point.x, local_point.y, body_.getWidth(), body_.getHeight());

	ofPushMatrix();
	ofTranslate(local_point.x, local_point.y, 0);
	ofRotateZ(angle_);
	ofTranslate(-local_point.x, -local_point.y, 0);
	ofDrawRectangle(draw_rect);
	ofPopMatrix();

	drawSensors();
}

void racingai::Car::updateSensorValues() {
	for (Sensor& sensor : sensors_) {
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
		}
		else {
			sensor.value = sensor.length;
		}
	}

}

void racingai::Car::drawSensors() {
	ofSetColor(66, 244, 212);
	for (Sensor sensor : sensors_) {
		ofPoint local_point1 = world_->getLocalPoint(sensor.input.p1.x, sensor.input.p1.y);
		ofPoint local_point2 = world_->getLocalPoint(sensor.input.p2.x, sensor.input.p2.y);
		//ofPolyline draw_line;

		//draw_line.addVertex(local_point1.x, local_point1.y);
		//draw_line.addVertex(local_point2.x, local_point2.y);

		ofDrawLine(local_point1, local_point2);
		//draw_line.draw();
	}
}

void racingai::Car::initSensors() {

	//middle
	Sensor middle_sensor;
	middle_sensor.length = 120;

	middle_sensor.value = middle_sensor.length;
	double x_m = ofGetWindowWidth() / 2 + WIDTH / 2 * cos(angle_ * PI / 180);
	double y_m = ofGetWindowHeight() / 2 + WIDTH / 2 * sin(angle_ * PI / 180);

	ofPoint global_point_m = world_->getGlobalPoint(x_m, y_m);
	b2Vec2 p1_m(global_point_m.x, global_point_m.y);
	b2Vec2 p2_m = p1_m + middle_sensor.length * b2Vec2(cos(angle_ * PI / 180), sin(angle_ * PI / 180));

	middle_sensor.input.p1 = p1_m;
	middle_sensor.input.p2 = p2_m;
	middle_sensor.input.maxFraction = 1.0;
	sensors_.push_back(middle_sensor);

	//Left
	Sensor left_sensor;
	left_sensor.length = 120;

	left_sensor.value = left_sensor.length;
	double x_l = ofGetWindowWidth() / 2 + WIDTH / 2 * cos(angle_ * PI / 180) + HEIGHT / 2 * sin(angle_ * PI / 180);
	double y_l = ofGetWindowHeight() / 2 + WIDTH / 2 * sin(angle_ * PI / 180) - HEIGHT / 2 * cos(angle_ * PI / 180);

	ofPoint global_point_l = world_->getGlobalPoint(x_l, y_l);
	b2Vec2 p1_l(global_point_l.x, global_point_l.y);
	b2Vec2 p2_l = p1_l + left_sensor.length * b2Vec2(cos((angle_ - 25) * PI / 180), sin((angle_ - 25) * PI / 180));

	left_sensor.input.p1 = p1_l;
	left_sensor.input.p2 = p2_l;
	left_sensor.input.maxFraction = 1.0;
	sensors_.push_back(left_sensor);

	//Right
	Sensor right_sensor;
	right_sensor.length = 120;
	right_sensor.value = right_sensor.length;

	double x_r = ofGetWindowWidth() / 2 + WIDTH / 2 * cos(angle_ * PI / 180) - HEIGHT / 2 * sin(angle_ * PI / 180);
	double y_r = ofGetWindowHeight() / 2 + WIDTH / 2 * sin(angle_ * PI / 180) + HEIGHT / 2 * cos(angle_ * PI / 180);

	ofPoint global_point_r = world_->getGlobalPoint(x_r, y_r);
	b2Vec2 p1_r(global_point_r.x, global_point_r.y);
	b2Vec2 p2_r = p1_r + right_sensor.length * b2Vec2(cos((angle_ + 25) * PI / 180), sin((angle_ + 25) * PI / 180));

	right_sensor.input.p1 = p1_r;
	right_sensor.input.p2 = p2_r;
	right_sensor.input.maxFraction = 1.0;
	sensors_.push_back(right_sensor);
}

void racingai::Car::updateSensors() {

	ofPoint local_point = world_->getLocalPoint(body_.getPosition());
	//middle
	double x_m = local_point.x + WIDTH / 2 * cos(angle_ * PI / 180);
	double y_m = local_point.y + WIDTH / 2 * sin(angle_ * PI / 180);

	ofPoint global_point_m = world_->getGlobalPoint(x_m, y_m);
	b2Vec2 p1_m(global_point_m.x, global_point_m.y);
	b2Vec2 p2_m = p1_m + sensors_[0].length * b2Vec2(cos(angle_ * PI / 180), sin(angle_ * PI / 180));

	sensors_[0].input.p1 = p1_m;
	sensors_[0].input.p2 = p2_m;

	//Left
	double x_l = local_point.x + WIDTH / 2 * cos(angle_ * PI / 180) + HEIGHT / 2 * sin(angle_ * PI / 180);
	double y_l = local_point.y + WIDTH / 2 * sin(angle_ * PI / 180) - HEIGHT / 2 * cos(angle_ * PI / 180);

	ofPoint global_point_l = world_->getGlobalPoint(x_l, y_l);
	b2Vec2 p1_l(global_point_l.x, global_point_l.y);
	b2Vec2 p2_l = p1_l + sensors_[1].length * b2Vec2(cos((angle_ - 25) * PI / 180), sin((angle_ - 25) * PI / 180));

	sensors_[1].input.p1 = p1_l;
	sensors_[1].input.p2 = p2_l;

	//Right
	double x_r = local_point.x + WIDTH / 2 * cos(angle_ * PI / 180) - HEIGHT / 2 * sin(angle_ * PI / 180);
	double y_r = local_point.y + WIDTH / 2 * sin(angle_ * PI / 180) + HEIGHT / 2 * cos(angle_ * PI / 180);

	ofPoint global_point_r = world_->getGlobalPoint(x_r, y_r);
	b2Vec2 p1_r(global_point_r.x, global_point_r.y);
	b2Vec2 p2_r = p1_r + sensors_[2].length * b2Vec2(cos((angle_ + 25) * PI / 180), sin((angle_ + 25) * PI / 180));

	sensors_[2].input.p1 = p1_r;
	sensors_[2].input.p2 = p2_r;
}

void racingai::Car::setSpeed(double s) {
	speed_ = s;
}

void racingai::Car::resetState() {
	dead_ = false;
	angle_ = 0;
	body_.setPosition(0, 0);
	position_ = ofPoint(0, 0);
	score_ = 0;
	body_.setVelocity(cos(angle_ * PI / 180) * speed_, sin(angle_ * PI / 180) * speed_);
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

ofxBox2dRect racingai::Car::getBody() {
	return body_;
}

double racingai::Car::getAngle() {
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


vector<racingai::Sensor> racingai::Car::getSensors() const {
	return sensors_;
}

vector<float> racingai::Car::getSensorValues() const {
	vector<float> values;
	for (Sensor sensor : sensors_) {
		values.push_back(sensor.value);
	}
	return values;
}


void racingai::Car::contactStart(ofxBox2dContactArgs &e) {
	if (e.a != NULL && e.b != NULL) {
		if (e.a->GetType() == b2Shape::e_polygon && e.b->GetType() == b2Shape::e_polygon) {

			ContactData * aData = (ContactData*)e.a->GetBody()->GetUserData();
			ContactData * bData = (ContactData*)e.b->GetBody()->GetUserData();

			if (aData) {
				aData->setTrue();
			}

			if (bData) {
				bData->setTrue();
			}
		}
	}
}

//--------------------------------------------------------------
void racingai::Car::contactEnd(ofxBox2dContactArgs &e) {
	if (e.a != NULL && e.b != NULL) {

		ContactData * aData = (ContactData*)e.a->GetBody()->GetUserData();
		ContactData * bData = (ContactData*)e.b->GetBody()->GetUserData();

		if (aData) {
			aData->setFalse();
		}

		if (bData) {
			bData->setFalse();
		}
	}
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
