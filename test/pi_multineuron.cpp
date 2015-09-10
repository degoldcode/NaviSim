/*
 * pi_multinoise.cpp
 *
 *  Created on: 04.08.2015
 *      Author: meicron
 */


#include "../src/simulation.h"
#include "../src/timer.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
using namespace std;

Simulation* sim;
const int numagents= 1;
const int numtrials= 1000;
const double T= 1000.;
const double dt= 0.1;


vector<int> neurons = {3}; //6, 9, 18, 36, 90, 180 0.5, 1.};
ofstream nrmse;

int main(){
	Timer timer(true);

	nrmse.open("data/nrmse_neuron.dat");
	for(int i = 0; i < neurons.size(); i++){
		printf("Start simulation with %u neurons.\n", neurons[i]);
		sim = new Simulation(numtrials, numagents, true);
		sim->SILENT = true;
		sim->init_controller(neurons[i], 0.02, 0.00, 0.00);
		sim->run(numtrials, T, dt);
		nrmse << neurons[i] << "\t" << sim->total_pi_error.mean() << "\t" << sim->total_pi_error.stddev()  << endl;
		delete sim;
	}
	nrmse.close();

	auto elapsed_secs_cl = timer.Elapsed();
	printf("%4.3f s. Done.\n", elapsed_secs_cl.count()/1000.);
}
