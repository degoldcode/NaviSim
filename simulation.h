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
#include <cmath>
#include "environment.h"
#include "navicontrol.h"

class Simulation {
public:
	Simulation(double param_start, double param_end, double dparam, int num_trials);
	~Simulation();

	void run_sim();
	void run_inbound();
	void run_outbound();

	double bound_angle(double phi);
	double inv_angle(double angle);
	double in_degr(double angle);

private:
	NaviControl* controller;
	Environment* environment;

	ofstream distor;
	ofstream gvlearn;
	ofstream lvlearn;

	// Simulation parameters
	int total_runs;
	bool inbound_on;

	// Controller parameters
	int num_neurons;
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
	double PI_angular_error;
	double PI_linear_error;
	int num_goalhits;
	int num_homing;
};



#endif /* SIMULATION_H_ */
