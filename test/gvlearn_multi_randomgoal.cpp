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
const int numtrials= 1000;
const double T= 450.;		//Maybe 600
const double Thome= 300.;	//Maybe 400
const double dt= 0.1;

int main(){
	Timer timer(true);

	sim = new Simulation(numtrials, numagents, true);
	sim->homing(false);
	sim->gvlearn(true);
	sim->beta(true);
	sim->lvlearn(false);
	sim->init_controller(18, 1, 1, 0.05, 0.0, 0.00, 0.0);
	sim->set_inward(int(Thome/dt));							// inward after 1000 seconds

	sim->run(numtrials, T, dt);
	delete sim;

	auto elapsed_secs_cl = timer.Elapsed();
	printf("%4.3f s. Done.\n", elapsed_secs_cl.count()/1000.);
}


