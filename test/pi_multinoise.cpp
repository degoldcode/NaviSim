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

vector<double> noise =  {0.01, 0.05, 0.1};//{0.0, 0.005, 0.01, 0.02, 0.05, 0.1, 0.2, 0.4}; // 0.5, 1.};
vector<string> str_noise =  {"data/error001.dat", "data/error005.dat", "data/error010.dat"};
ofstream nrmse;

int main(){
	Timer timer(true);

	nrmse.open("data/nrmse_noise.dat");
	for(int i = 0; i < noise.size(); i++){
		printf("Start simulation with %g %% sensory noise.\n", noise[i]*100);
		sim = new Simulation(numtrials, numagents, false, str_noise, i);
		sim->SILENT=true;
		sim->init_controller(18, noise[i], 0.00, 0.00);
		sim->run(numtrials, T, dt);
		nrmse << noise[i] << "\t" << sim->total_pi_error.mean() << "\t" << sim->total_pi_error.stddev()  << endl;
		delete sim;
	}
	nrmse.close();

	auto elapsed_secs_cl = timer.Elapsed();
	printf("%4.3f s. Done.\n", elapsed_secs_cl.count()/1000.);
}
