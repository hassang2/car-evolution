#pragma once
#include <ctime>
#include <cstdlib>
#include <utility>
#include <vector>

#include "ofMain.h"
#include "car.h"
#include "track.h"
#include "ofxBox2d.h"
#include "world.h"

#include "../NEAT/neat.h"
#include "../NEAT/population.h"
#include "../NEAT/experiments.h"

namespace racingai {


	// Enum to represent the current state of the game
	enum GameState {
		IN_PROGRESS = 0,
		PAUSED,
		FINISHED
	};

	class carGame : public ofBaseApp {
		int MAX_GENS = 10;
	private:
		GameState current_state_ = IN_PROGRESS; // The current state of the game, used to determine possible actions
		Track game_track_; // the track object used in the game
		vector<shared_ptr<Car> > cars_;
		ofxBox2d* box2d_;           // the box2d world
		Universe world_;
		NEAT::Neat neat_;
		
		//Score tracking
		std::vector<int> top_scores_;
		int lowest_score_ = INT_MAX;

		//Controls
		bool right_btn_hold_ = false;
		bool left_btn_hold_ = false;

		// Private helper methods to render various aspects of the game on screen.
		void drawGameOver();
		void drawGamePaused();
		void drawTopScores();

		void initBox2d();

		// Resets the game objects to their original state.
		void reset();

		//Score Tracking
		void evaluateScore();
		int findLowestScoreIndex();
	public:

		// Function used for one time setup
		void setup();

		// Main event loop functions called on every frame
		void update();
		void draw();

		// Event driven functions, called on appropriate user action
		void keyPressed(int key);
		void keyReleased(int key);
		void mousePressed(int x, int y, int button);
		void mouseDragged(int x, int y, int button);
		//void windowResized(int w, int h);
	};
} // namespace racingai
