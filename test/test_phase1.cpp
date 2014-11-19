/*
 * test_phase1.cpp
 *
 *  Created on: Nov 18, 2014
 *      Author: degoldschmidt
 */

#include "../src/simulation.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
using namespace std;

Simulation* sim;//Environment* env;
const int numagents= 1;
const int numtrials= 1;
const double T= 1000.;
const double dt= 0.1;
//int tstep= 0;
//ofstream agent_str;

int main(){
	//agent_str.open("data/agent.dat");
	sim = new Simulation(numagents, true);//env = new Environment(numagents);
	sim->init_controller();

	sim->run(numtrials, T, dt);

	delete sim;
}
