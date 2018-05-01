#pragma once
#include "ofApp.h"
#include <iostream>

using namespace racingai;

// Setup method
void carGame::setup() {
	ofSetLogLevel(OF_LOG_ERROR);
	ofBackgroundHex(0x1F2C30);
	ofSetRectMode(OF_RECTMODE_CENTER);     // 0,0 is at center of a rectangle now
	ofSetWindowTitle("AI Racing");

	initBox2d();
	
	game_car_ = new Car();
	game_car_->setup(box2d_);
	game_track_.setup(box2d_, game_car_, "../tracks/track1");
	//game_track_.loadTrack("../tracks/track1");

	srand(static_cast<unsigned>(time(0))); // Seed random with current time
}

/*
Update function called before every draw
If the function should update when it is called it will:
1. Check to see if the game is in progress, if it is paused or over it should not update.
2. Check to see if the current head of the snake intersects the food pellet. If so:
* The snake should grow by length 1 in its current direction
* The food should be moved to a new random location
3. Update the snake in the current direction it is moving
4. Check to see if the snakes new position has resulted in its death and the end of the game
*/
void carGame::update() {
	box2d_->update();

	if (current_state_ == IN_PROGRESS) {
		//ofCircle car_rect(game_car_.getXPos(), game_car_.getYPos(), 5.0, 5.0);
		if (right_btn_hold_) game_car_->swerveRight();
		if (left_btn_hold_) game_car_->swerveLeft();

		//Order matters : track should be updated first
		game_track_.update();
		game_car_->update();

		//if (game_car_.isDead()) {
		//	current_state_ = FINISHED;
		//	evaluateScore();
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
	game_car_->draw();

	for (int i = 0; i < circles_.size(); i++) {
		ofFill();
		ofSetHexColor(0x90d4e3);
		circles_[i].get()->draw();
	}
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

	ofDrawBitmapString("Score: " + std::to_string(game_track_.getScore()) , ofGetWindowWidth() / 2 - 10, 30);
}


/*
Function that handles actions based on user key presses
1. if key == F12, toggle fullscreen
2. if key == p and game is not over, toggle pause
3. if game is in progress handle WASD action
4. if key == r and game is over reset it

WASD logic:
Let dir be the direction that corresponds to a key
if current direction is not dir (Prevents key spamming to rapidly update the snake)
and current_direction is not opposite of dir (Prevents the snake turning and eating itself)
Update direction of snake and force a game update (see ofApp.h for why)
*/
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
			if (game_car_->getSpeed() > 1.0) game_car_->setSpeed(0.0);
			else game_car_->setSpeed(3.0);
			break;
		case 'P':
			game_track_.toggleScoreLineEdit();
			break;
		case 'L':
			game_track_.removeEdge();
			break;
		case 'R':
			reset();
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
	// delete current car?
	game_car_ = new Car();
	current_state_ = IN_PROGRESS;
}

void racingai::carGame::evaluateScore() {
	if (current_state_ == FINISHED) {

		int total_score = game_car_->getScore();

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

//void carGame::windowResized(int w, int h) {
//	game_food_.resize(w, h);
//	game_snake_.resize(w, h);
//}

void carGame::drawGameOver() {
	string lose_message = "You Lost! Final Score: " + game_car_->getScore();
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
	box2d_->setGravity(0, 0);
	box2d_->createGround();
	box2d_->setFPS(60.0);
	//box2d_->registerGrabbing();
}

void carGame::drawGamePaused() {
	string pause_message = "P to Unpause!";
	ofSetColor(0, 0, 0);
	ofDrawBitmapString(pause_message, ofGetWindowWidth() / 4, 50);
	drawTopScores();
}
