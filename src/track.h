#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"
namespace racingai {

	//struct Piece {
	//	ofPolyline body;	
	//};


	class Track {
	private:
		ofVec2f position_;
		ofPolyline groundLine_;  //the shape of ground
		ofxBox2dEdge ground_;    //the ground object
		ofxBox2d* box2d_;        //box2d engine

	public:
		Track();
		void update();
		void setup(ofxBox2d* box);
		void draw();
		ofxBox2dEdge getGround() const;
		vector<ofPolyline> getSegments();
	};

} // namespace racingai