# Project Proposal

For my final project I will be doing a machine learning project. Specifically I will be using Neuroevolution of Augmented Topologies, NEAT for short, which is a genetic algorithm based on a [paper](http://nn.cs.utexas.edu/downloads/papers/stanley.ec02.pdf) by Kenneth Stanley in 2002 used to evolve neural networks to do complex tasks. Having this paper already read, I will use this algortihm to train my network to play a simple 2d track racing game. 

An example of an AI evolved by this algorithm is [here](https://youtu.be/qv6UVOQ0F44)
My project probably will not look as complex and impressive but I will try to come up with something original and interesting to deploy my network on.


## Libraries:
#### NEAT C++
I will be using NEAT C++ [library](http://nn.cs.utexas.edu/?neat-c) to evolve my network.

#### Game Library
This is unlikely, but I might use a library to develop my game. This will depend on how using a library will make things for me easier and if it is worth my time to learn how to use the library. I am saying this is unlikely because I believe openFrameWorks also provides some nice interfances for developing games and graphical applications which is probably enough for my needs.

## The Game

The game will be a top view of a car track and the user has to finish the designated amount of loops as fast as possible ([example](https://duskfire.files.wordpress.com/2011/08/pyracerz.png). The game will have three modes: playing the game yourself and trying to beat the high score, watching the AI that I have trained play, or competing against the AI. There will be no collision in the game becasue that's the setting I am training my network in. The game will start with simple controls such as only pressing keys to go right or left and as I go on, depening on how much time I have remaining to complete the project, I will add more controls and mechanics such as a break and boost system or obsticles to avoid. The goal of this project will be to train an AI that can surpass human performance in playing the game.

## Training the Network

To train the network I will come up with parameters in the game that I want to feed to the network. Currently from what I've seen from other projects of similar nature, the best parameters are the distance of the wall in front of the car at different angles. After evaluating the inputs, the network will give outputs of what button is best to press. The fitness of my genetic algorithm will be how far a car got into the track and how long it took for it to get there. 

