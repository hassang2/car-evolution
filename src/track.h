#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"
#include "car.h"
namespace racingai {

	class Track {
	private:
		ofVec2f position_;
		Car* game_car_;

		vector <shared_ptr<ofxBox2dEdge> > edges_;
		vector <shared_ptr<ofxBox2dRect>> rectangles_;
		ofPolyline score_line_ = ofPolyline();
		ofxBox2d* box2d_;        //box2d engine
		
		double global_x_ = 0;
		double global_y_ = 0;
	
		bool score_line_edit_ = false;   //flag if score line editing is enabled/disabled
	
	public:
		Track();
		void update();
		void setup(ofxBox2d* box, Car* car, string file_path);
		void draw();
		void center();
		void addEdge();
		void removeEdge();
		void toggleScoreLineEdit();
		//adds an edge to the track
		void addPoint(int x, int y);

		//removes the last drawn edge of the track
		void removePoint();
		void loadTrack(std::string path);
		void saveTrack() const;
		void setupRectangles();
		//int getGlobalX() const;
		//int getGlobalY() const;
		void setGlobalX(double x);
		void setGlobalY(double y);
		int getScore();
		//converts window coordinates to global coordinates
		ofPoint getGlobalPoint(double x, double y) const;
		ofPoint getGlobalPoint(ofPoint point) const;
		//converts global coordinates to window coordinates
		ofPoint getLocalPoint(double x, double y) const;
		ofPoint getLocalPoint(ofPoint point) const;
	};

} // namespace racingai