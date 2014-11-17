/*****************************************************************************
 *  simulation.h                                                             *
 *                                                                           *
 *  Created on:   Aug 31, 2014                                               *
 *  Author:       Dennis Goldschmidt                                         *
 *  Email:        goldschmidtd@ini.phys.ethz.ch                              *
 *                                                                           *
 *                                                                           *
 *  Copyright (C) 2014 by Dennis Goldschmidt                                 *
 *                                                                           *
 *  This file is part of the program NaviSim                                 *
 *                                                                           *
 *  NaviSim is free software: you can redistribute it and/or modify          *
 *  it under the terms of the GNU General Public License as published by     *
 *  the Free Software Foundation, either version 3 of the License, or        *
 *  (at your option) any later version.                                      *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU General Public License for more details.                             *
 *                                                                           *
 *  You should have received a copy of the GNU General Public License        *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
 *                                                                           *
 ****************************************************************************/

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <armadillo>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include "environment.h"
#include "navicontrol.h"


/**
 * Simulation Class
 *
 * 	This class creates and handles simulations
 * 	on a trial- and cycle-based level
 *
 */

class Simulation {
public:

	/**
	 * Constructor
	 *
	 *	@param (string) in_param_type: string of parameter specification (for parameter scan)
	 *	@int (int) in_num_trials: number of subsequent trials
	 *
	 */
	Simulation(string in_param_type, int in_num_trials);

	/**
	 * Destructor
	 *
	 */
	~Simulation();


	//************ Class functions ************//

	/**
	 * Returns angle within bounds [-PI,PI]
	 *
	 * 	@param (double) angle: input angle
	 * 	@return (double)
	 */
	double bound(double angle);

	/**
	 * Returns controller
	 *
	 * 	@return (NaviControl*)
	 */
	NaviControl* get_controller();

	/**
	 * Returns environment
	 *
	 * 	@return (Environment*)
	 */
	Environment* get_environment();

	/**
	 * Returns last run
	 *
	 * 	@return (int)
	 */
	int get_lrun();

	/**
	 * Returns success rate
	 *
	 * 	@return (double)
	 */
	double get_srate();

	/**
	 * Returns angle in degrees [0,360)
	 *
	 * 	@param (double) angle: input angle
	 * 	@return (double)
	 */
	double in_degr(double angle);

	/**
	 * Returns inversed angle [-PI,PI)
	 *
	 * 	@param (double) angle: input angle
	 * 	@return (double)
	 */
	double inv_angle(double angle);

	/**
	 * Resets the simulation for each trial
	 *
	 * @return (void)
	 */
	void reset();

	/**
	 * Runs the inbound foraging
	 *
	 * 	@return (void)
	 */
	void run_inbound();

	/**
	 * Runs the outbound foraging
	 *
	 * 	@return (void)
	 */
	void run_outbound();

	/**
	 * Runs the simulation
	 *
	 * 	@return (void)
	 */
	void run();

	/**
	 * Runs the simulation
	 *
	 * 	@return (void)
	 */
	void set_param();

	//************ Controller parameters ************//

	int num_neurons = 360;							// Number of array neurons
	double sens_noise = 0.0;						// Sensory noise
	double pi_leakage = 0.0;						// PI leakage
	int num_motivs = 2;								// number of motifs
	double motor_command = 0.0;						// motor command

	//************ Evaluation parameters ************//

	vector< running_stat<double> > success_rate_avg;
	vector< running_stat<double> > explor_rate_avg;
	vector< running_stat<double> > success_rate_avg2;	//YELLOW
	vector< running_stat<double> > explor_rate_avg2;
	vector< running_stat<double> > prob_B;
	vector< running_stat<double> > prob_Y;
	vector< running_stat<int> > choice;
	running_stat<double> total_reward;
	mat gv_history0;
	mat gv_history1;

private:

	//************ Class objects ************//

	NaviControl* controller;
	Environment* environment;

	//************ Simulation parameters ************//

	int total_runs;
	const bool inbound_on = true;					// if inbound is simulated
	const double max_outbound_time = 100.;				// maximum time for outbound foraging
	const double max_inbound_time = max_outbound_time;	// maximum time for inbound foraging
	string file_ext;								// string for file name extension

	//************ Environment parameters ************//

	const int agents = 1;							// number of agents
	const double m_rad = 25.;						// maximum radius of objects
	const double g_dens = 0.025;					// density of goals
	const int ngs = int(g_dens*(M_PI*pow(m_rad,2)));// number of goals
	const double lm_dens = 0.05;					// density of landmarks
	const int nlms = int(lm_dens*(M_PI*pow(m_rad,2)));// number of landmarks
	const bool rand_env = false;						// option random environment

	//************ Output file streams ************//

	ofstream distor;
	ofstream PI_results;
	ofstream PI_resultseach;
	ofstream endpoints;
	ofstream gvlearn;
	ofstream lvlearn;
	ofstream stats_gl;
	ofstream statsall_gl;
	ofstream gv_angl;
	stringstream file_name;

	//************ Evaluation parameters ************//

	running_stat<double> PI_angular_error;
	running_stat<double> PI_linear_error;
	int last_run = 0;
	double success_rate = 0;
	int blue_hits = 0;
	int yellow_hits = 0;
	int num_goalhits = 0;
	int num_homing = 0;
	int run_div = int(total_runs/4.);

	//************ Timing parameters ************//

	double global_time;		// continuous time during multiple cycles
	//double trial_time;		// continuous time for each trial
	const double dt = 0.1;	// integration time
	int timestep;			// discrete time steps
};



#endif /* SIMULATION_H_ */
