#pragma once
#include "ofApp.h"
#include <iostream>

#include "../NEAT/experiments.h"

using namespace racingai;

// Setup method
void carGame::setup() {
	srand(static_cast<unsigned>(time(0))); // Seed random with current time

	ofSetLogLevel(OF_LOG_ERROR);
	ofBackgroundHex(0x1F2C30);
	ofSetRectMode(OF_RECTMODE_CENTER);     // 0,0 is at center of a rectangle now
	ofSetWindowTitle("AI Racing");

	initBox2d();
	
	world_ = Universe();

	//game_track_ = Track();

	//cars_.push_back(make_shared<Car>());
	//cars_.push_back(make_shared<Car>());

	neat_.init(MAX_GENS);

	//collect car objects from organsisms to be used in the game
	//vector<NEAT::Organism*>::iterator curorg;
	//for (curorg = neat_.getpopulation()->organisms.begin(); curorg != (neat_.getpopulation()->organisms).end(); ++curorg) {
	//	cars_.push_back((*curorg)->getCar());
	//}

	vector<Car*> beep;
	//beep.push_back((*neat_.getpopulation()->organisms.begin())->getCar());
	beep.push_back(new Car());
	//cars_.push_back(new Car());
	//test.push_back(10);
	//std::cout << test.size() << std::endl;
	game_track_.setup(box2d_, beep, &world_, "../tracks/track4");

	//neat();
}

void carGame::update() {
	box2d_->update();

	if (current_state_ == IN_PROGRESS) {
		//Order matters : track should be updated first
		game_track_.update();
		
		if (!game_track_.getFocusCar()->isDead()) {
			if (right_btn_hold_) game_track_.getFocusCar()->swerveRight();
			if (left_btn_hold_) game_track_.getFocusCar()->swerveLeft();

			game_track_.getFocusCar()->update();
		}

		bool finish = true;
		for (Car* car : cars_) {
			if (!car->isDead()) {
				finish = false;
				break;
			}
		}
		//if (finish) {
		//	if (neat_.evalPopulation()) {
		//		std::cout << "Optimal car Found";
		//	} else if (neat_.getCurrentGen() < MAX_GENS) {
		//		reset();
		//	}
		//}
	}
}


/*
Draws the current state of the game with the following logic
1. If the game is paused draw the pause screen
2. If the game is finished draw the game over screen and final score
3. Draw the current position of the food and of the snake
*/
void carGame::draw() {
	if (current_state_ == PAUSED) {
		drawGamePaused();
	}
	else if (current_state_ == FINISHED) {
		drawGameOver();
	}

	game_track_.draw();
	//for (Car* car : cars_) {
	//	car->draw();
	//}
	if (!game_track_.getFocusCar()->isDead()) game_track_.getFocusCar()->draw();


	//draw FPS
	ofSetColor(255);
	ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 10, 10);

	ofDrawBitmapString("(" + ofToString(ofGetMouseX()) + ", " + ofToString(ofGetMouseY()) + ")", 10, 30);
	ofDrawBitmapString("[A] move left", 10, 50);
	ofDrawBitmapString("[D] move right", 10, 70);
	ofDrawBitmapString("[N] add new track edge", 10, 90);
	ofDrawBitmapString("[P] add score line", 10, 110);
	ofDrawBitmapString("[L] remove last curve", 10, 130);
	ofDrawBitmapString("[right mouse] draw", 10, 150);
	ofDrawBitmapString("[left mouse] remove last point", 10, 170);
	ofDrawBitmapString("[B] save track", 10, 190);
	ofDrawBitmapString("[Q] quit", 10, 210);

	ofDrawBitmapString("Score: " + std::to_string(game_track_.getFocusCar()->getScore()) , ofGetWindowWidth() / 2 - 10, 30);
}

void carGame::keyPressed(int key) {
	if (key == OF_KEY_F12) {
		ofToggleFullscreen();
		return;
	}

	int upper_key = toupper(key); // Standardize on upper case
	if (current_state_ == IN_PROGRESS) {
		switch (upper_key) {
		case 'W':
			break;
		case 'A':
			right_btn_hold_ = true;
			break;
		case 'D':
			left_btn_hold_ = true;
			break;
		case 'S':
			if (game_track_.getFocusCar()->getSpeed() > 0.0) {
				game_track_.getFocusCar()->setSpeed(0.0);
			} else if (!game_track_.getFocusCar()->isDead()) {
				game_track_.getFocusCar()->setSpeed(3.0);
			}
			break;
		case 'P':
			game_track_.toggleScoreLineEdit();
			break;
		case 'L':
			game_track_.removeEdge();
			break;
		case 'R':
			break;
		case 'B':
			game_track_.saveTrack();
			break;
		case 'N':
			game_track_.addEdge();
			break;
		case 'Q':
			std::exit(0);
		}
	}
}

void carGame::keyReleased(int key) {
	int upper_key = toupper(key); // Standardize on upper case

	if (current_state_ == IN_PROGRESS) {
		if (upper_key == 'A' || key == OF_KEY_LEFT) {
			right_btn_hold_ = false;
		}
		else if (upper_key == 'D' || key == OF_KEY_RIGHT) {
			left_btn_hold_ = false;
		}
	}
}

void carGame::mousePressed(int x, int y, int button) {
	if (button == OF_MOUSE_BUTTON_LEFT) game_track_.addPoint(x, y);
	else if (button == OF_MOUSE_BUTTON_RIGHT) game_track_.removePoint();
}

void carGame::mouseDragged(int x, int y, int button) {
	game_track_.addPoint(x, y);
}



void carGame::reset() {	
	Car sample;
	for (Car* car : cars_) {
		car->resetState();
	}

	cars_.clear();
	vector<NEAT::Organism*>::iterator curorg;
	for (curorg = neat_.getpopulation()->organisms.begin(); curorg != (neat_.getpopulation()->organisms).end(); ++curorg) {
		cars_.push_back((*curorg)->getCar());
	}

	game_track_.setCars(cars_);

	current_state_ = IN_PROGRESS;
}

void racingai::carGame::evaluateScore() {
	if (current_state_ == FINISHED) {
		int total_score = game_track_.getFocusCar()->getScore();

		if (top_scores_.size() < 10) {
			top_scores_.push_back(total_score);
			if (total_score < lowest_score_) {
				lowest_score_ = total_score;
			}
		}
		else if (total_score > lowest_score_) {
			int lowest_score_index = findLowestScoreIndex();
			top_scores_[lowest_score_index] = total_score;
			lowest_score_ = top_scores_[findLowestScoreIndex()];
		}
	}
}

int racingai::carGame::findLowestScoreIndex() {
	if (top_scores_.size() == 0) return 0;
	int lowest = top_scores_[0];
	int index = 0;
	for (int i = 1; i < top_scores_.size(); i++) {
		if (top_scores_[i] < lowest) {
			lowest = top_scores_[i];
			index = i;
		}
	}
	return index;
}

void carGame::drawGameOver() {
	string lose_message = "You Lost! Final Score: " + game_track_.getFocusCar()->getScore();
	ofSetColor(0, 0, 0);
	ofDrawBitmapString(lose_message, ofGetWindowWidth() / 4, 50);
	drawTopScores();
}

void carGame::drawTopScores() {
	ofDrawBitmapString("Top Scores:\n", ofGetWindowWidth() / 2 + 100, ofGetWindowHeight() / 2 - 200);

	for (int i = 0; i < top_scores_.size(); i++) {
		std::string score_message = std::to_string(i + 1) + ": " + std::to_string(top_scores_[i]);
		ofDrawBitmapString(score_message, ofGetWindowWidth() / 2 + 100, ofGetWindowHeight() / 2 - 25 * (6 - i));
	}
}

void racingai::carGame::initBox2d() {
	box2d_ = new ofxBox2d();
	box2d_->init();
	box2d_->enableEvents();   // <-- turn on the event listener
	box2d_->setGravity(0, 0);
	box2d_->createGround();
	box2d_->setFPS(60.0);
}

void carGame::drawGamePaused() {
	string pause_message = "P to Unpause!";
	ofSetColor(0, 0, 0);
	ofDrawBitmapString(pause_message, ofGetWindowWidth() / 4, 50);
	drawTopScores();
}
