#include "experiments.h"
#include <cstring>

#define NO_SCREEN_OUT

//Perform evolution on XOR, for gens generations
Population *xor_test(int gens) {
    Population *pop = 0;
    Genome *start_genome;
    char curword[20];
    int id;

    ostringstream *fnamebuf;
    int gen;

    int evals[NEAT::num_runs];  //Hold records for each run
    int genes[NEAT::num_runs];
    int nodes[NEAT::num_runs];
    int winnernum;
    int winnergenes;
    int winnernodes;
    //For averaging
    int totalevals = 0;
    int totalgenes = 0;
    int totalnodes = 0;
    int expcount;
    int samples;  //For averaging

    memset(evals, 0, NEAT::num_runs * sizeof(int));
    memset(genes, 0, NEAT::num_runs * sizeof(int));
    memset(nodes, 0, NEAT::num_runs * sizeof(int));

    ifstream iFile("xorstartgenes", ios::in);

    cout << "START XOR TEST" << endl;

    cout << "Reading in the start genome" << endl;
    //Read in the start Genome
    iFile >> curword;
    iFile >> id;
    cout << "Reading in Genome id " << id << endl;
    start_genome = new Genome(id, iFile);
    iFile.close();

    for (expcount = 0; expcount < NEAT::num_runs; expcount++) {
        //Spawn the Population
        cout << "Spawning Population off Genome2" << expcount << endl;

        pop = new Population(start_genome, NEAT::pop_size);

        cout << "Verifying Spawned Pop" << endl;
        pop->verify();

        for (gen = 1; gen <= gens; gen++) {
            cout << "Epoch " << gen << endl;

            //This is how to make a custom filename
            fnamebuf = new ostringstream();
            (*fnamebuf) << "gen_" << gen << ends;  //needs end marker

#ifndef NO_SCREEN_OUT
            cout << "name of fname: " << fnamebuf->str() << endl;
#endif

            char temp[50];
            sprintf(temp, "gen_%d", gen);

            //Check for success
            //if successful make this the last loop
            if (xor_epoch(pop, gen, temp, winnernum, winnergenes, winnernodes)) {
                //	if (xor_epoch(pop,gen,fnamebuf->str(),winnernum,winnergenes,winnernodes)) {
                //Collect Stats on end of experiment
                evals[expcount] = NEAT::pop_size * (gen - 1) + winnernum;
                genes[expcount] = winnergenes;
                nodes[expcount] = winnernodes;
                gen = gens;
            }

            //Clear output filename
            fnamebuf->clear();
            delete fnamebuf;
        }

        if (expcount < NEAT::num_runs - 1) delete pop;

    }

    //Average and print stats
    cout << "Nodes: " << endl;
    for (expcount = 0; expcount < NEAT::num_runs; expcount++) {
        cout << nodes[expcount] << endl;
        totalnodes += nodes[expcount];
    }

    cout << "Genes: " << endl;
    for (expcount = 0; expcount < NEAT::num_runs; expcount++) {
        cout << genes[expcount] << endl;
        totalgenes += genes[expcount];
    }

    cout << "Evals " << endl;
    samples = 0;
    for (expcount = 0; expcount < NEAT::num_runs; expcount++) {
        cout << evals[expcount] << endl;
        if (evals[expcount] > 0) {
            totalevals += evals[expcount];
            samples++;
        }
    }

    cout << "Failures: " << (NEAT::num_runs - samples) << " out of " << NEAT::num_runs << " runs" << endl;
    cout << "Average Nodes: " << (samples > 0 ? (double) totalnodes / samples : 0) << endl;
    cout << "Average Genes: " << (samples > 0 ? (double) totalgenes / samples : 0) << endl;
    cout << "Average Evals: " << (samples > 0 ? (double) totalevals / samples : 0) << endl;

    return pop;

}

bool xor_evaluate(Organism *org) {
    Network *net;
    double out[4]; //The four outputs
    double this_out; //The current output
    int count;
    double errorsum;

    bool success;  //Check for successful activation
    int numnodes;  /* Used to figure out how many nodes
		    should be visited during activation */

    int net_depth; //The max depth of the network to be activated
    int relax; //Activates until relaxation

    //The four possible input combinations to xor
    //The first number is for biasing
    double in[4][3] = {{1.0, 0.0, 0.0},
                       {1.0, 0.0, 1.0},
                       {1.0, 1.0, 0.0},
                       {1.0, 1.0, 1.0}};

    net = org->net;
    numnodes = ((org->gnome)->nodes).size();

    net_depth = net->max_depth();

    //TEST CODE: REMOVE
    //cout<<"ACTIVATING: "<<org->gnome<<endl;
    //cout<<"DEPTH: "<<net_depth<<endl;

    //Load and activate the network on each input
    for (count = 0; count <= 3; count++) {
        net->load_sensors(in[count]);

        //Relax net and get output
        success = net->activate();

        //use depth to ensure relaxation
        for (relax = 0; relax <= net_depth; relax++) {
            success = net->activate();
            this_out = (*(net->outputs.begin()))->activation;
        }

        out[count] = (*(net->outputs.begin()))->activation;

        net->flush();

    }

    if (success) {
        errorsum = (fabs(out[0]) + fabs(1.0 - out[1]) + fabs(1.0 - out[2]) + fabs(out[3]));
        org->fitness = pow((4.0 - errorsum), 2);
        org->error = errorsum;
    } else {
        //The network is flawed (shouldn't happen)
        errorsum = 999.0;
        org->fitness = 0.001;
    }

#ifndef NO_SCREEN_OUT
    cout << "Org " << (org->gnome)->genome_id << "                                     error: " << errorsum << "  ["
         << out[0] << " " << out[1] << " " << out[2] << " " << out[3] << "]" << endl;
    cout << "Org " << (org->gnome)->genome_id << "                                     fitness: " << org->fitness
         << endl;
#endif

    //  if (errorsum<0.05) {
    //if (errorsum<0.2) {
    if ((out[0] < 0.5) && (out[1] >= 0.5) && (out[2] >= 0.5) && (out[3] < 0.5)) {
        org->winner = true;
        return true;
    } else {
        org->winner = false;
        return false;
    }

}

int xor_epoch(Population *pop, int generation, char *filename, int &winnernum, int &winnergenes, int &winnernodes) {
    vector<Organism *>::iterator curorg;
    vector<Species *>::iterator curspecies;
    //char cfilename[100];
    //strncpy( cfilename, filename.c_str(), 100 );

    //ofstream cfilename(filename.c_str());

    bool win = false;


    //Evaluate each organism on a test
    for (curorg = (pop->organisms).begin(); curorg != (pop->organisms).end(); ++curorg) {
        if (xor_evaluate(*curorg)) {
            win = true;
            winnernum = (*curorg)->gnome->genome_id;
            winnergenes = (*curorg)->gnome->extrons();
            winnernodes = ((*curorg)->gnome->nodes).size();
            if (winnernodes == 5) {
                //You could dump out optimal genomes here if desired
//                (*curorg)->gnome->print_to_filename("xor_optimal");
//                cout<<"DUMPED OPTIMAL"<<endl;
            }
        }
    }

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
    if (win ||
        ((generation % (NEAT::print_every)) == 0))
        pop->print_to_file_by_species(filename);


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

    pop->epoch(generation);

    if (win) return 1;
    else return 0;
}

Population *opposite_test(int gens) {

    Population *pop = 0;
    Genome *start_genome;
    char curword[20];
    int id;

    ostringstream *fnamebuf;
    int gen;

    int evals[NEAT::num_runs];  //Hold records for each run
    int genes[NEAT::num_runs];
    int nodes[NEAT::num_runs];
    int winnernum;
    int winnergenes;
    int winnernodes;
    //For averaging
    int totalevals = 0;
    int totalgenes = 0;
    int totalnodes = 0;
    int expcount;
    int samples;  //For averaging

    memset(evals, 0, NEAT::num_runs * sizeof(int));
    memset(genes, 0, NEAT::num_runs * sizeof(int));
    memset(nodes, 0, NEAT::num_runs * sizeof(int));

    ifstream iFile("xorstartgenes2", ios::in);

    cout << "START opposite TEST" << endl;

    cout << "Reading in the start genome" << endl;
    //Read in the start Genome
    iFile >> curword;
    iFile >> id;

    cout << "Reading in Genome id " << id << endl;
    start_genome = new Genome(id, iFile);
    iFile.close();

    for (expcount = 0; expcount < NEAT::num_runs; expcount++) {
        //Spawn the Population
        cout << "Spawning Population off Genome2" << expcount << endl;

        pop = new Population(start_genome, NEAT::pop_size);

        cout << "Verifying Spawned Pop" << endl;
        pop->verify();

        for (gen = 1; gen <= gens; gen++) {
            cout << "Epoch " << gen << endl;

            //This is how to make a custom filename
            fnamebuf = new ostringstream();
            (*fnamebuf) << "gen_" << gen << ends;  //needs end marker

#ifndef NO_SCREEN_OUT
            cout << "name of fname: " << fnamebuf->str() << endl;
#endif

            char temp[50];
            sprintf(temp, "gen_%d", gen);

            //Check for success
            //if successful make this the last loop
            if (opposite_epoch(pop, gen, temp, winnernum, winnergenes, winnernodes)) {
                //	if (xor_epoch(pop,gen,fnamebuf->str(),winnernum,winnergenes,winnernodes)) {
                //Collect Stats on end of experiment
                evals[expcount] = NEAT::pop_size * (gen - 1) + winnernum;
                genes[expcount] = winnergenes;
                nodes[expcount] = winnernodes;
                gen = gens;
            }

            //Clear output filename
            fnamebuf->clear();
            delete fnamebuf;
        }

        if (expcount < NEAT::num_runs - 1) delete pop;

    }

    //Average and print stats
    cout << "Nodes: " << endl;
    for (expcount = 0; expcount < NEAT::num_runs; expcount++) {
        cout << nodes[expcount] << endl;
        totalnodes += nodes[expcount];
    }

    cout << "Genes: " << endl;
    for (expcount = 0; expcount < NEAT::num_runs; expcount++) {
        cout << genes[expcount] << endl;
        totalgenes += genes[expcount];
    }

    cout << "Evals " << endl;
    samples = 0;
    for (expcount = 0; expcount < NEAT::num_runs; expcount++) {
        cout << evals[expcount] << endl;
        if (evals[expcount] > 0) {
            totalevals += evals[expcount];
            samples++;
        }
    }

    cout << "Failures: " << (NEAT::num_runs - samples) << " out of " << NEAT::num_runs << " runs" << endl;
    cout << "Average Nodes: " << (samples > 0 ? (double) totalnodes / samples : 0) << endl;
    cout << "Average Genes: " << (samples > 0 ? (double) totalgenes / samples : 0) << endl;
    cout << "Average Evals: " << (samples > 0 ? (double) totalevals / samples : 0) << endl;

    return pop;

}

bool opposite_evaluate(Organism *org) {
    Network *net;
    double out[50];
    double this_out; //The current output
    int count;
    double errorsum;

    bool success;  //Check for successful activation
    int numnodes;  /* Used to figure out how many nodes
		    should be visited during activation */

    int net_depth; //The max depth of the network to be activated
    int relax; //Activates until relaxation

    //The first number is for biasing
    double **in = new double*[50];
    for (int i = 0;i < 50; i++) {
        in[i] = new double[2];
    }

    double *expected = new double[50];

    for (int i = 0; i < 50; i++) {
        double x = -25 +  i;
        in[i][0] = 1.0;
        in[i][1] = x;

        if (x > 0) {
            expected[i] = 1.0;
        } else {
            expected[i] = 0.0;
        }
    }


    net = org->net;
    numnodes = ((org->gnome)->nodes).size();

    net_depth = net->max_depth();

    //TEST CODE: REMOVE
    //cout<<"ACTIVATING: "<<org->gnome<<endl;
    //cout<<"DEPTH: "<<net_depth<<endl;

    //Load and activate the network on each input
    for (count = 0; count < 50; count++) {
        net->load_sensors(in[count]);

        //Relax net and get output
        success = net->activate();

        //use depth to ensure relaxation
        for (relax = 0; relax <= net_depth; relax++) {
            success = net->activate();
            this_out = (*(net->outputs.begin()))->activation;
        }

        out[count] = (*(net->outputs.begin()))->activation;

        net->flush();
    }

    if (success) {
        errorsum = 0;
        for (int i = 0; i < 50; i++) {
            errorsum += fabs(expected[i] - out[i]);
        }
        org->fitness = pow((50 - errorsum), 2);
        org->error = errorsum;
    } else {
        //The network is flawed (shouldn't happen)
        cout << "unsuccessful activation";
        errorsum = 999.0;
        org->fitness = 0.001;
    }

#ifndef NO_SCREEN_OUT
    cout << "Org " << (org->gnome)->genome_id << "                                     error: " << errorsum << "  ["
         << out[0] << " " << out[1] << " " << out[2] << " " << out[3] << "]" << endl;
    cout << "Org " << (org->gnome)->genome_id << "                                     fitness: " << org->fitness
         << endl;
#endif


//  if (errorsum<0.05) {
    if (errorsum < 0.1) {
        std::cout << "winner found. enter number: " << std::endl;
//    if ((out[0] < 0.5) && (out[1] >= 0.5) && (out[2] >= 0.5) && (out[3] < 0.5)) {
        double x;
        double output;
        cin >> x;
        double input[2] = {1.0, x};
        net->load_sensors(input);
        //Relax net and get output
        success = net->activate();

        //use depth to ensure relaxation
        for (relax = 0; relax <= net_depth; relax++) {
            success = net->activate();
            this_out = (*(net->outputs.begin()))->activation;
        }

        output = (*(net->outputs.begin()))->activation;

        if (output < .5) {
            std::cout << "negative" <<std::endl;
        } else {
            std::cout << "positive" <<std::endl;
        }
        net->flush();

        org->winner = true;
        return true;
    } else {
        org->winner = false;
        return false;
    }


}

int opposite_epoch(Population *pop, int generation, char *filename, int &winnernum, int &winnergenes, int &winnernodes) {
    vector<Organism *>::iterator curorg;
    vector<Species *>::iterator curspecies;
    //char cfilename[100];
    //strncpy( cfilename, filename.c_str(), 100 );

    //ofstream cfilename(filename.c_str());

    bool win = false;


    //Evaluate each organism on a test
    for (curorg = (pop->organisms).begin(); curorg != (pop->organisms).end(); ++curorg) {
        if (opposite_evaluate(*curorg)) {
            win = true;
            winnernum = (*curorg)->gnome->genome_id;
            winnergenes = (*curorg)->gnome->extrons();
            winnernodes = ((*curorg)->gnome->nodes).size();
            if (winnernodes == 5) {
                //You could dump out optimal genomes here if desired
//                (*curorg)->gnome->print_to_filename("xor_optimal");
//                cout<<"DUMPED OPTIMAL"<<endl;
            }
        }
    }

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
    if (win ||
        ((generation % (NEAT::print_every)) == 0))
        pop->print_to_file_by_species(filename);


    if (win) {
        for (curorg = (pop->organisms).begin(); curorg != (pop->organisms).end(); ++curorg) {
            if ((*curorg)->winner) {
                cout << "WINNER IS #" << ((*curorg)->gnome)->genome_id << endl;
                //Prints the winner to file
                //IMPORTANT: This causes generational file output!
                print_Genome_tofile((*curorg)->gnome, "opposite_winner");
            }
        }

    }

    pop->epoch(generation);

    if (win) return 1;
    else return 0;
}