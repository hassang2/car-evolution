#pragma once
#include "ofMain.h"
namespace racingai {

	//struct Piece {
	//	ofPolyline body;	
	//};


	class Track {
	private:
		ofVec2f position_;
		std::vector<ofPolyline> segments_;

	public:
		Track();
		void setup();
		vector<ofPolyline> getSegments();
	};

} // namespace racingai