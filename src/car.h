#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"


namespace racingai {

	struct Sensor{
		b2RayCastInput input;
		b2RayCastOutput output;
		double value;
		double length;
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
		ofVec2f getBodySize() const; // gets the size of a body segment, used for rendering
		bool isDead() const; // Determines if the current state of the snake is dead
		void update(); // updates the snake one body square in the current direction
		void resize(int w, int h); // Resizes the snake to a new width and height
		int getScore() const; // Gets the number of food items the snake has eaten
		void swerveRight(); //swerves the car to right
		void swerveLeft(); //swerves the car to left
		void draw();
		void setup(ofxBox2d* box);
		void setSpeed(double s);
		void initSensors();
		void updateSensorValues();

		ofxBox2dRect* getBody() const;
		double getXPos() const;
		double getYPos() const;
		double getAngle() const;
		double getSpeed() const;
		double getHeight() const;
		double getWidth() const;
	};
} // namespace racingai

