#include "experiments.h"
#include <cstring>


#define NO_SCREEN_OUT


void NEAT::Neat::init(int gens) {

	//Loading paramets from file
	//***********RANDOM SETUP***************//
	/* Seed the random-number generator with current time so that
	the numbers will be different every time we run.    */
	//srand((unsigned)time(NULL));

	//Load in the params
	if (NEAT::load_neat_params("../NEAT/test.ne", true)) {
		cout << "Loaded the Paramaters" << endl;
	}
	else {
		cout << "COULD NOT LOAD THE PARAMETERS" << endl;
	}

	NEAT::Genome *start_genome;
	char curword[20];
	int id;

	generations_ = gens;

	//For averaging
	int totalevals = 0;
	int totalgenes = 0;
	int totalnodes = 0;
	int expcount;
	int samples;  //For averaging

	ifstream iFile("../NEAT/car_ai_start_genome", ios::in);

	cout << "START CAR AI" << endl;

	cout << "Reading in the start genome" << endl;
	//Read in the start Genome
	iFile >> curword;
	iFile >> id;
	cout << "Reading in Genome id " << id << endl;
	start_genome = new NEAT::Genome(id, iFile);
	iFile.close();

	//Spawn the Population
	cout << "Spawning Population off Genome" << endl;

	pop = new NEAT::Population(start_genome, NEAT::pop_size);

	cout << "Verifying Spawned Pop..." << endl;
	pop->verify();
	cout << "Verified" << endl;
}

int NEAT::Neat::evalPopulation() {
	vector<Organism*>::iterator curorg;
	vector<Species*>::iterator curspecies;
	//char cfilename[100];
	//strncpy( cfilename, filename.c_str(), 100 );

	//ofstream cfilename(filename.c_str());

	bool win = false;


	std::cout << "Evaluating Organisms..." << std::endl;
	//Evaluate each organism on a test
	for (curorg = (pop->organisms).begin(); curorg != (pop->organisms).end(); ++curorg) {
		if (evalOrganism(*curorg)) {
			win = true;
			winnernum = (*curorg)->gnome->genome_id;
			winnergenes = (*curorg)->gnome->extrons();
			winnernodes = ((*curorg)->gnome->nodes).size();
			if (winnernodes == 5) {
				//You could dump out optimal genomes here if desired
				//(*curorg)->gnome->print_to_filename("xor_optimal");
				//cout<<"DUMPED OPTIMAL"<<endl;
			}
		}
	}

	std::cout << "Evaluation Complete" << std::endl;


	//Average and max their fitnesses for dumping to file and snapshot
	for (curspecies = (pop->species).begin(); curspecies != (pop->species).end(); ++curspecies) {

		//This experiment control routine issues commands to collect ave
		//and max fitness, as opposed to having the snapshot do it, 
		//because this allows flexibility in terms of what time
		//to observe fitnesses at

		(*curspecies)->compute_average_fitness();
		(*curspecies)->compute_max_fitness();
	}


	//Take a snapshot of the population, so that it can be
	//visualized later on
	//if ((generation%1)==0)
	//  pop->snapshot();

	//Only print to file every print_every generations
	char temp[50];
	sprintf(temp, "gen_%d", current_gen_);
	//if (win ||
	//	((current_gen_ % (NEAT::print_every)) == 0))
	//	pop->print_to_file_by_species(temp);


	if (win) {
		for (curorg = (pop->organisms).begin(); curorg != (pop->organisms).end(); ++curorg) {
			if ((*curorg)->winner) {
				cout << "WINNER IS #" << ((*curorg)->gnome)->genome_id << endl;
				//Prints the winner to file
				//IMPORTANT: This causes generational file output!
				print_Genome_tofile((*curorg)->gnome, "xor_winner");
			}
		}

	}


	pop->epoch(current_gen_);
	current_gen_++;

	if (win) return 1;
	else return 0;
}

void NEAT::Neat::getOrganismOutput(Organism * org) {
	Network *net;
	//double out[4]; //The four outputs
	double this_out[3]; //The current output
	int count;

	bool success;  //Check for successful activation
	int numnodes;  /* Used to figure out how many nodes
				   should be visited during activation */

	int net_depth; //The max depth of the network to be activated
	int relax; //Activates until relaxation

			   //The four possible input combinations to xor
			   //The first number is for biasing

	net = org->net;
	numnodes = ((org->gnome)->nodes).size();

	net_depth = net->max_depth();

	//TEST CODE: REMOVE
	//cout<<"ACTIVATING: "<<org->gnome<<endl;
	//cout<<"DEPTH: "<<net_depth<<endl;

	//Load and activate the network on each input

	vector<float> sensor_values = org->getCar()->getSensorValues();

	net->load_sensors(sensor_values);

	//Relax net and get output
	success = net->activate();

	//use depth to ensure relaxation
	for (relax = 0; relax <= net_depth; relax++) {
		success = net->activate();
		for (int i = 0; i < net->outputs.size(); i++) {
			this_out[i] = net->outputs[i]->activation;
		}
	}

	for (int i = 0; i < net->outputs.size(); i++) {
		this_out[i] = net->outputs[i]->activation;
	}

	//??? IS this NEEDED?
	//out[count] = (*(net->outputs.begin()))->activation;

	net->flush();

	int max_index = 0;
	double max_value = this_out[0];
	for (int i = 1; i < 3; i++) {
		if (this_out[i] > max_value) {
			max_value = this_out[i];
			max_index = i;
		}
	}

	switch (max_index) {
	case 1:
		org->getCar()->swerveLeft();
		break;
	case 2:
		org->getCar()->swerveRight();
		break;
	case 3:
		break;
	}

	if (!success) cout << "Flawed Network" << std::endl;

}

bool NEAT::Neat::evalOrganism(Organism* org) {
	org->fitness = org->getCar()->getScore();
	org->error = 1.0 / org->fitness;

	if (org->fitness > 300) {
		org->winner = true;
		return true;
	}
	else {
		org->winner = false;
		return false;
	}
}
