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
const double Thome= 400.; // 2000 for 10m
const double T= 1.5*Thome; // 600 for 2m, 3000 for 10m
const double dt= 0.1;

int main(){
	Timer timer(true);

	sim = new Simulation(numtrials, numagents, false);
	sim->add_goal(0., 5., 0);
	sim->homing(true);
	sim->gvlearn(true);
	sim->beta(true);
	sim->init_controller(18, 0.05, 0.0, 0.00);
	sim->set_inward(int(Thome/dt));						

	sim->c(0)->set_expl(0, 0.0, true);
	sim->c(0)->GV_module()->GV(0,Vec(5.,0.),true);
	sim->run(numtrials, T, dt);
	delete sim;

	auto elapsed_secs_cl = timer.Elapsed();
	printf("%4.3f s. Done.\n", elapsed_secs_cl.count()/1000.);
}


