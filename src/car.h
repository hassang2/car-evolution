#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"

namespace racingai {

	class Car {
		double SWERVE_AMOUNT = 5.0;
		double WIDTH = 40.0;
		double HEIGHT = 20.0;
	private:
		double angle_ = 0.0;
		double speed_ = 1.5;
		ofVec2f position_;
		ofxBox2dRect* body_;

		double score_ = 0.0;

		ofVec2f screen_dims_; // The current screen dimensions (needed to calculate values on resize()
		static const float kbody_size_modifier_; // The proportion of the screen width a body square is
		ofVec2f body_size_; // the size of a snake body piece based on kbody_size_modifier_

	public:
		Car();
		ofVec2f getBodySize() const; // gets the size of a body segment, used for rendering
		bool isDead() const; // Determines if the current state of the snake is dead
		void update(); // updates the snake one body square in the current direction
		void resize(int w, int h); // Resizes the snake to a new width and height
		int getScore() const; // Gets the number of food items the snake has eaten
		void swerveRight(); //swerves the car to right
		void swerveLeft(); //swerves the car to left
		ofxBox2dRect* getBody() const;
		double getXPos() const;
		double getYPos() const;
		double getAngle() const;
		double getHeight() const;
		double getWidth() const;
		bool intersects(ofPolyline line) const;
	};
} // namespace racingai
