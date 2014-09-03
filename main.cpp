/*
 * main.cpp
 *
 *  Created on: 01.08.2014
 *      Author: meicron
 */

#include <armadillo>
#include <fstream>
#include "simulation.h"
#include "timer.h"
using namespace std;
using namespace arma;

Simulation* my_sim;

int main(){
	Timer timer(true);

	my_sim = new Simulation(number_of_neurons, 0.0, 1.0, 0.1,100);
	my_sim->run_sim();
	delete my_sim;

	auto elapsed_secs_cl = timer.Elapsed();
	printf("%4.3f s. Done.\n", elapsed_secs_cl.count()/1000.);
}


