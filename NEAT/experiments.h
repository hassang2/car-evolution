#pragma once

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

namespace NEAT {

	class Neat {
	private: 
		int winnernum;
		int winnergenes;
		int winnernodes;

		NEAT::Population* pop;
		int generations_;
		int current_gen_ = 1;

		bool evalOrganism(Organism* org);
	public:
		//loads the parameters, reads the starting genomes, creates population, starts the algorithm
		void init(int gens);

		//handles speciation
		//if an optimal organism was found it returns true, dumps winner poppulation data into a file, and stops
		//if not, it loads up the next generation
		int evalPopulation();

		//evalues the fitness of the organism, runs the inputs through the network
		void getOrganismOutput(Organism *org);

		Population* getpopulation() { return pop; }
		int getCurrentGen() { return current_gen_; }

	};
} //namespace NEAT






