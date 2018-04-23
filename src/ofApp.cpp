#include "ofApp.h"
#include <iostream>

using namespace racingai;

// Setup method
void carGame::setup() {
	box2d.init();
	box2d.setGravity(0, 5);
	box2d.createGround();
	box2d.setFPS(60.0);
	box2d.registerGrabbing();

	ofSetWindowTitle("AI Racing");
	ofSetRectMode(OF_RECTMODE_CENTER);     // 0,0 is at center of a rectangle now
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
	box2d.update();

	if (current_state_ == IN_PROGRESS) {
		//ofCircle car_rect(game_car_.getXPos(), game_car_.getYPos(), 5.0, 5.0);
		if (right_btn_hold_) game_car_.swerveRight();
		if (left_btn_hold_) game_car_.swerveLeft();
		game_car_.update();
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
	drawTrack();
	drawCar();

	for (int i = 0; i < circles.size(); i++) {
		ofFill();
		ofSetHexColor(0x90d4e3);
		circles[i].get()->draw();
	}
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

	if (upper_key == 'P' && current_state_ != FINISHED) {
		// Pause or unpause
		current_state_ = (current_state_ == IN_PROGRESS) ? PAUSED : IN_PROGRESS;
		drawTopScores();
	}
	else if (current_state_ == IN_PROGRESS) {

		// If current direction has changed to a valid new one, force an immediate update and skip the next frame update
		//if (upper_key == 'W' && current_direction != DOWN && current_direction != UP) {
		//	game_snake_.setDirection(UP);
		//	update();
		//	should_update_ = false;
		//}
		if (upper_key == 'A' || key == OF_KEY_LEFT) {
			circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
			circles.back().get()->setPhysics(3.0, 0.53, 0.1);
			circles.back().get()->setup(box2d.getWorld(), 200, 200, 10);
			right_btn_hold_ = true;
		}
		//else if ((upper_key == 'S') && current_direction != UP && current_direction != DOWN) {
		//	game_snake_.setDirection(DOWN);
		//	update();
		//	should_update_ = false;
		//}
		else if (upper_key == 'D' || key == OF_KEY_RIGHT) {
			left_btn_hold_ = true;
		} else if (upper_key == 'R') {
			reset();
		} else if (upper_key == 'Q') {
			std::exit(0);
		}
	}

}

void carGame::keyReleased(int key) {
	int upper_key = toupper(key); // Standardize on upper case

	if (current_state_ == IN_PROGRESS) {
		if (upper_key == 'A' || key == OF_KEY_LEFT) {
			right_btn_hold_ = false;
		} else if (upper_key == 'D' || key == OF_KEY_RIGHT) {
			left_btn_hold_ = false;
		}
	}
}

void carGame::reset() {
	game_car_ = Car();
	current_state_ = IN_PROGRESS;
}

void racingai::carGame::evaluateScore() {
	if (current_state_ == FINISHED) {

		int total_score = game_car_.getScore();

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

void carGame::drawCar() {
	ofSetColor(ofColor(150, 40, 255));

	//handles rotation as well
	ofPushMatrix();
	ofTranslate(game_car_.getXPos(), game_car_.getYPos(), 0);
	ofRotateZ(game_car_.getAngle());
	ofTranslate(-game_car_.getXPos(), -game_car_.getYPos(), 0);
	ofDrawRectangle(game_car_.getXPos(), game_car_.getYPos(), game_car_.getWidth(), game_car_.getHeight());
	ofPopMatrix();
}

void carGame::drawGameOver() {
	string lose_message = "You Lost! Final Score: " + game_car_.getScore();
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

void racingai::carGame::drawTrack() {
	game_track_.getSegments()[0].draw();
}

void carGame::drawGamePaused() {
	string pause_message = "P to Unpause!";
	ofSetColor(0, 0, 0);
	ofDrawBitmapString(pause_message, ofGetWindowWidth() / 4, 50);
	drawTopScores();
}
