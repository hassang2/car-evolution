#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"
#include "world.h"

namespace racingai {

	struct Sensor{
		b2RayCastInput input;
		double value;
		double length;
		shared_ptr<ofPolyline> line = shared_ptr<ofPolyline>();
		void setValue(double v) { value = v; }
	};

	//a class to register contacts in the game
	class ContactData {
	private:
		bool in_contact = false;

	public:
		void setFalse() { in_contact = false; }
		void setTrue() { in_contact = true; }
		bool isInContact() { return in_contact; }
	};

	class Car {
		double SWERVE_AMOUNT = 6.0;
		double WIDTH = 40.0;
		double HEIGHT = 20.0;

	private:
		bool dead_ = false;
		double angle_ = 0.0;
		double speed_ = 20.0;
		ofPoint position_;
		ofxBox2dRect body_;
		ofxBox2d* box2d_;
		Universe* world_;
		double score_ = 0.0;
		vector<Sensor> sensors_;

	public:
		//Car
		Car();
		//Car(const Car &source);
		~Car();
		//Car &operator=(const Car &source);      // Copy assignment operator

		void setup(ofxBox2d* box, Universe* world);
		void update(); // updates the snake one body square in the current direction
		void swerveRight(); //swerves the car to right
		void swerveLeft(); //swerves the car to left
		void draw();
		void setSpeed(double s);
		void resetState();
		void setScore(int s) { score_ = s; }
		//sensors
		void initSensors();
		void updateSensorValues();
		void drawSensors();
		void updateSensors();

		//contact listeners
		void contactStart(ofxBox2dContactArgs &e);
		void contactEnd(ofxBox2dContactArgs &e);

		//Getters
		ofxBox2dRect getBody();
		double getAngle();
		double getSpeed() const;
		double getHeight() const;
		double getWidth() const;
		vector<Sensor> getSensors() const;
		vector<float> getSensorValues() const;
		bool isDead() { return dead_; }
		int getScore() const; // Gets the number of food items the snake has eaten
		ofPoint getPosition() { return position_; }
	};
	
	class RayCaster : public b2RayCastCallback {
	public:
		float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);
		b2Vec2 getIntersectionPoint() const;
		bool didCollide() const;

	private:
		bool collide = false;
		b2Vec2 intersection_point_;
	};
} // namespace racingai

