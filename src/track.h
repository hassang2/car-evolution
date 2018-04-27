#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"
#include "car.h"
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
		Car* game_car_;
		vector<ofPoint> vertices_;
		vector<ofPoint> added_vertices_;
		double global_x_ = ofGetWindowWidth() / 2;
		double global_y_ = ofGetWindowHeight() / 2;
	public:
		Track();
		void update();
		void setup(ofxBox2d* box, Car* car);
		void draw();
		void center();

		//adds an edge to the track
		void addPoint(int x, int y);

		//removes the last drawn edge of the track
		void removePoint();

		void loadTrack(std::string path);
		void saveTrack() const;
		void printPaths() const;
		//int getGlobalX() const;
		//int getGlobalY() const;
		void setGlobalX(double x);
		void setGlobalY(double y);

		//converts window coordinates to global coordinates
		ofPoint getGlobalPoint(double x, double y);

		//converts global coordinates to window coordinates
		ofPoint getLocalPoint(double x, double y);
		ofxBox2dEdge getGround() const;
		vector<ofPolyline> getSegments();
	};

} // namespace racingai