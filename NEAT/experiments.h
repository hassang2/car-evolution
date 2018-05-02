#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include "neat.h"
#include "network.h"
#include "population.h"
#include "organism.h"
#include "genome.h"
#include "species.h"

using namespace std;

using namespace NEAT;

//loads the parameters, reads the starting genomes, creates population, starts the algorithm
Population  *carAI(int gens);

//handles speciation and loads up the next population
//dumps winner poppulation data into files
int AI_epoch(Population *pop, int generation, char *filename, int &winnernum, int &winnergenes, int &winnernodes);

//evalues the fitness of the organism, runs the inputs through the network
bool aiEvaluate(Organism *org);


#endif






