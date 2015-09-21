/*
 * test_pi_single.cpp
 *
 *  Created on: 03.08.2015
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
const int numtrials= 50;
const double T= 1500.;
const double dt= 0.1;

int main(){
	Timer timer(true);

	sim = new Simulation(numtrials, numagents, true);
	sim->homing(false);
	sim->gvlearn(true);
	sim->init_controller(18, 0.05, 0.0, 0.00, 50.0);
	sim->set_inward(int(1000./dt));							// inward after 1000 seconds

	sim->run(numtrials, T, dt);
	delete sim;

	auto elapsed_secs_cl = timer.Elapsed();
	printf("%4.3f s. Done.\n", elapsed_secs_cl.count()/1000.);
}


