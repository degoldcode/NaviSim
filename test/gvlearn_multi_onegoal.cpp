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
const double T= 2000.;
const double Thome= 1500.;
const double dt= 0.1;

int main(){
	Timer timer(true);

	sim = new Simulation(numtrials, numagents, false);
	sim->add_goal(-5., 0., 0);
	//sim->add_goal(-0., 5., 0);
	sim->homing(true);
	sim->gvlearn(true);
	sim->init_controller(18, 0.05, 0.00, 0.00, 0.0);
	sim->set_inward(int(Thome/dt));							// inward after 1000 seconds

	sim->run(numtrials, T, dt);
	delete sim;

	auto elapsed_secs_cl = timer.Elapsed();
	printf("%4.3f s. Done.\n", elapsed_secs_cl.count()/1000.);
}


