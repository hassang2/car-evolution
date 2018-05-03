#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"
#include "car.h"
#include "world.h"

namespace racingai {

	class Track {
	private:
		vector <Car*> cars_;
		Car* focus_car_;
		Universe* world_;
		vector <shared_ptr<ofxBox2dEdge> > edges_;
		vector <shared_ptr<ofxBox2dRect>> rectangles_;
		ofPolyline score_line_ = ofPolyline();
		ofxBox2d* box2d_;        //box2d engine
	
		bool score_line_edit_ = false;   //flag if score line editing is enabled/disabled
	
	public:
		Track();
		void update();
		void setup(ofxBox2d* box, vector <Car*> cars_list, Universe* world, string file_path);
		void draw();
		void center();
		void addEdge();
		void removeEdge();
		void toggleScoreLineEdit();

		//sets the cars sets the focus_car to the first index of the list
		void setCars(vector <Car*> new_cars);
		//adds an edge to the track
		void addPoint(int x, int y);

		//removes the last drawn edge of the track
		void removePoint();
		void loadTrack(std::string path);
		void saveTrack() const;
		void setupRectangles();

		Car* getFocusCar() { return focus_car_; }
	};

} // namespace racingai