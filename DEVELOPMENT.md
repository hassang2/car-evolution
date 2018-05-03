# Dev Logs

## NEAT:
### LOG
#### Week 1:
* cleared the experiments .cpp and .h files.
* added a test experiment to classify numbers as positive or negative.
* after much effort, the positive-negative classifier is working now with 100% accuracy.

#### Week 2
* Integrated NEAT with my car game: all the cpp files now compile 
* Edited the functions in experiments file: Added Neat class.
* functions in experiments were edited to work with the game: changed fitness function.
* Modified organism class: added car* variable to store the car instance of the organisms
* The evolution starts (with problems)

### TODO
* Make sure the evolution is happening and the cars are getting better
* Properly save the data of the ideal car to be loaded for later use
* Fix segmentation fault when trying to pass vector of car pointers to the track setup function

## Race Game:
### LOG
#### Week 1:
* Copy pasted code from snake game.
* Modified the pasted code to fit my game (added car/ai/track classes)
* code compiles and shows a straight line and circle.
* Circle now can move to right and left with A or D or left/right arrows
* changed the circle to rectangle.

#### Week 2:
* Rectnagle now rotates properly when it turns right or left
* Now using box2d physics engine for collisions
* car collision is now working
* added track edditing
* added track saving and loading
* Reworked the track and car classses so it supports multi car drawing
* Reworked track centering so the track is fixed and the cars move and the other way
* Added score system with a score line
* Added car sensors
* Implemented ray casting for car sensors to determine their values
* Implemented contact listener for car class to kill it when it collides with a wall

### TODO

* Move the car so it is inside the track
* Add modes: Play single player or play with AI or watch ai (evolve?)


