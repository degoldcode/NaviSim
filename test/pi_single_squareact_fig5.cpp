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
const int numtrials= 1;
const double T= 200.;
const double dt= 0.1;

int main(){
	Timer timer(true);

	sim = new Simulation(numtrials, numagents, false);
	sim->add_pipe(0.,-5.,0.,0.);
	sim->add_pipe(-5.,-5.,0.,-5.);
	sim->add_pipe(-5.,0.,-5.,-5.);
	sim->add_pipe(0.,0.,-5.,-0.2);
	sim->homing(true);
	sim->init_controller(18, 0.0, 0.0, 0.00);
	sim->set_inward(int(100./dt));							// inward after 1000 seconds
	sim->run(numtrials, T, dt);
	delete sim;

	auto elapsed_secs_cl = timer.Elapsed();
	printf("%4.3f s. Done.\n", elapsed_secs_cl.count()/1000.);
}


