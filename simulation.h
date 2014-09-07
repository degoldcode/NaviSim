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

enum param_types {

	//PI analysis
	number_of_neurons = 0,
	sensory_noise = 1,
	maximum_outbound_time = 2,
	exploration_factor = 3,
	memory_leakage = 4,
	response_function = 5,
	decoding_kernel = 6,

	//Homing analysis
	PI_steer_constant = 7,

	//GVL analysis
	env_goal_density = 8,
	learning_rate = 9,


	//LVL analysis
};


class Simulation {
public:
	Simulation(string in_param_type, int in_num_trials);
	~Simulation();

	void run_sim(double param);
	void run_inbound();
	void run_outbound();

	double bound_angle(double phi);
	double inv_angle(double angle);
	double in_degr(double angle);

	string get_param_type(param_types input);
	string get_param_name(param_types input);
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

private:
	NaviControl* controller;
	Environment* environment;

	ofstream distor;
	ofstream PI_results;
	ofstream PI_resultseach;
	ofstream gvlearn;
	ofstream lvlearn;
	stringstream file_name;

	// Evaluation
	running_stat<double> PI_angular_error;
	running_stat<double> PI_linear_error;
	int num_goalhits;
	int num_homing;
};



#endif /* SIMULATION_H_ */
