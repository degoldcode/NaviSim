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
const int numtrials= 500;
const double T= 100.;
const double dt= 0.1;

int main(){
	Timer timer(true);

	sim = new Simulation(numtrials, numagents, false);
	sim->add_goal(-2., 2., 0);
	sim->add_landmark(0., 1.);
	sim->add_landmark(-1., 1.);
	//sim->add_pipe(0., -1., 0., 0.);
	//sim->add_pipe(-8., -10., 2., 0.);

	sim->init_controller(18, 0.05, 0.0, 0.00);
	sim->set_inward(int(100./dt));							// inward after 1000 seconds
	sim->homing(true);
	sim->gvlearn(true);
	sim->lvlearn(true);

	sim->run(numtrials, T, dt);
	delete sim;

	auto elapsed_secs_cl = timer.Elapsed();
	printf("%4.3f s. Done.\n", elapsed_secs_cl.count()/1000.);
}


