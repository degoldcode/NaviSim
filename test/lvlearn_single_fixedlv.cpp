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
const int numagents  = 1;    /// number of agents
const int numtrials  = 100;  /// number of trials
const double T       = 900.; /// maximum time of a trial (500.)
const double Thome   = 600.; /// foraging time until agent (300.)
const double dt      = 0.1;  /// integration step size (Forward Euler method)

int main(){
	Timer timer(true);

	sim = new Simulation(numtrials, numagents, false);
	sim->add_goal(0., 9., 0);
	sim->add_landmark(2., 6.);
	sim->add_landmark(-1., 3.);
	sim->add_landmark(0., 0.);

	sim->homing(false);
	sim->gvlearn(true);
	sim->lvlearn(true);
	sim->init_controller(18, 1, 3, 0.05, 0.0, 0.00);

	sim->set_inward(int(Thome/dt));
	/*sim->c(0)->set_expl(0, 0.001, true);
	sim->c(0)->LV_module()->LV(0,Vec(-2.,3.),true);
	sim->c(0)->LV_value(0, 1.);
	sim->c(0)->LV_module()->LV(1,Vec(3.,3.),true);
	sim->c(0)->LV_value(1, 1.);
	sim->c(0)->LV_module()->LV(2,Vec(-1.,3.),true);
	sim->c(0)->LV_value(2, 1.);*/
	sim->run(numtrials, T, dt);
	delete sim;

	auto elapsed_secs_cl = timer.Elapsed();
	printf("%4.3f s. Done.\n", elapsed_secs_cl.count()/1000.);
}


