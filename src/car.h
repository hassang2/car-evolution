#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"


namespace racingai {

	struct Sensor{
		b2RayCastInput input;
		double value;
		double length;
		shared_ptr<ofPolyline> line = shared_ptr<ofPolyline>();
		void setValue(double v) { value = v; }
	};

	class Car {
		double SWERVE_AMOUNT = 4.0;
		double WIDTH = 40.0;
		double HEIGHT = 20.0;
	private:
		double angle_ = 0.0;
		double speed_ = 6.0;
		ofxBox2dRect* body_;
		ofxBox2d* box2d_;
		double score_ = 0.0;

		ofVec2f screen_dims_; // The current screen dimensions (needed to calculate values on resize()
		static const float kbody_size_modifier_; // The proportion of the screen width a body square is
		ofVec2f body_size_; // the size of a snake body piece based on kbody_size_modifier_

		vector<Sensor> sensors_;

	public:
		Car();
		void setup(ofxBox2d* box);

		void update(); // updates the snake one body square in the current direction
		int getScore() const; // Gets the number of food items the snake has eaten
		void swerveRight(); //swerves the car to right
		void swerveLeft(); //swerves the car to left
		void draw();
		void setSpeed(double s);

		//sensors
		void initSensors();
		void updateSensorValues();
		void drawSensors();

		ofVec2f getBodySize() const;
		ofxBox2dRect* getBody() const;
		double getXPos() const;
		double getYPos() const;
		double getAngle() const;
		double getSpeed() const;
		double getHeight() const;
		double getWidth() const;
		vector<Sensor> getSensors() const;
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

