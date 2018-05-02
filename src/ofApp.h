#pragma once
#include <ctime>
#include <cstdlib>
#include <utility>
#include <vector>

#include "ofMain.h"
#include "car.h"
#include "track.h"
#include "ofxBox2d.h"

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
	private:
		GameState current_state_ = IN_PROGRESS; // The current state of the game, used to determine possible actions
		Car* game_car_; // The object that represents the user controlled car
		Track game_track_; // the track object used in the game

		ofxBox2d* box2d_;           // the box2d world

		vector    <shared_ptr<ofxBox2dCircle> > circles_;         // default box2d circles

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
		int neat() {
			//list<NNode*> nodelist;

			int pause;

			NEAT::Population *p = 0;

			//***********RANDOM SETUP***************//
			/* Seed the random-number generator with current time so that
				the numbers will be different every time we run.    */
			srand((unsigned)time(NULL));

			//Load in the params
			if (NEAT::load_neat_params("../NEAT/test.ne", true)) {
				cout << "Loaded the Paramaters" << endl;
			} else {
				cout << "COULD NOT LOAD THE PARAMETERS" << endl;
			}

			int choice;

			//cout << "Please choose an experiment: " << endl;
			//cout << "1 - Car AI " << endl;
			//cout << " " << endl;
			//cout << " " << endl;
			//cout << " " << endl;
			//cout << "Number: ";

			//cin >> choice;
			//switch (choice) {
			//case 1:
			//	p = carAI(100);
			//	break;
			//case 4:
			//	break;
			//default:
			//	cout << "Not an available option." << endl;
			//}

			p = carAI(100);

			delete p;

			return (0);
		};
	};
} // namespace racingai
