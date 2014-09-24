/*
 * simulation.h
 *
 *  Created on: 31.08.2014
 *      Author: meicron
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <armadillo>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include "environment.h"
#include "navicontrol.h"


class Simulation {
public:
	Simulation(string in_param_type, int in_num_trials);
	~Simulation();

	void run_sim(double param);
	void run_inbound();
	void run_outbound();

	NaviControl* get_controller();
	Environment* get_environment();

	double bound_angle(double phi);
	double inv_angle(double angle);
	double in_degr(double angle);

	void set_param();

	// Simulation parameters
	int total_runs;
	bool inbound_on;
	int num_params;
	string file_ext;

	// Controller parameters
	int num_neurons;
	double sens_noise;
	double pi_leakage;
	int num_motivs;
	int max_outbound_time;
	int max_inbound_time;
	double motor_command;

	// Environment parameters
	int agents;
	double max_radius;
	double goal_density;
	int num_goals;
	double lm_density;
	int num_landmarks;

	// Evaluation
	int end_run;
	double success_rate;

private:
	NaviControl* controller;
	Environment* environment;

	ofstream distor;
	ofstream PI_results;
	ofstream PI_resultseach;
	ofstream endpoints;
	ofstream gvlearn;
	ofstream lvlearn;
	ofstream stats_gl;
	ofstream statsall_gl;
	stringstream file_name;

	// Evaluation
	running_stat<double> PI_angular_error;
	running_stat<double> PI_linear_error;

	int num_goalhits;
	int num_homing;
	int run_div;
};



#endif /* SIMULATION_H_ */
