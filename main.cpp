/*
 * main.cpp
 *
 *  Created on: 01.08.2014
 *      Author: meicron
 */

#include <armadillo>
#include <fstream>
#include <string>
#include "simulation.h"
#include "timer.h"
using namespace std;
using namespace arma;

const double param_start = 0.01;
const double param_end = 0.2;
const int max_idx = 6;
double param_array[max_idx]={0.0,0.01,0.02,0.05,0.1,0.2};
int idx = 0;
const double dparam = 0.01;
const int num_trials = 1000;
string param_type = "sensory_noise";

Simulation* my_sim;

int main(){
	Timer timer(true);
	my_sim = new Simulation(param_type, num_trials);
	double param = param_start;

	printf("Start simulation with varying %s from %g to %g in %g intervals. Averaged over %u trials.\n\n", param_type.c_str(), param_start, param_end, dparam, num_trials);
	while(param <= param_end && idx < max_idx){
		param = param_array[idx];
		my_sim->sens_noise = param;
		my_sim->run_sim(param);
		idx++;
		//param += dparam;
/*		if(param>0.02)
			param += dparam;
		if(param>0.08)
			param+=0.1;*/
	}
	delete my_sim;

	auto elapsed_secs_cl = timer.Elapsed();
	printf("%4.3f s. Done.\n", elapsed_secs_cl.count()/1000.);
}


