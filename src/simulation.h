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
#include <cstdio>
#include <cmath>
#include <vector>
#include "environment.h"
#include "controller.h"


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
	 *	@param (int) in_num_trials: number of subsequent trials
	 */
	Simulation(int in_numtrials, int in_agents, bool random_env);

	/**
	 * Destructor. Closes IO file streams.
	 *
	 */
	~Simulation();


	//************ Class functions ************//

	Agent* a(int i);

	/**
	 * Adds a goal at position (x,y) with color
	 *
	 * 	@param (double) x: x postion of goal
	 * 	@param (double) y: y postion of goal
	 * 	@param (int) color: color index of goal (default: 0)
	 * 	@param (double) size: reward amount available at goal (default: 1.)
	 * 	@param (bool) decay: true, if amount of reward decays (default: false)
	 * 	@return (void)
	 */
	void add_goal(double x, double y, int color = 0, double size = 1., bool decay=false);

	/**
	 * Adds a randomly placed goal
	 *
	 *	@param (double) max_radius: maximum radius of placement
	 * 	@return (void)
	 */
	void add_goal(double max_radius);

	/**
	 * Adds a landmark at position (x,y)
	 *
	 * 	@param (double) x: x postion of goal
	 * 	@param (double) y: y postion of goal
	 * 	@return (void)
	 */
	void add_landmark(double x, double y);

	/**
	 * Adds a pipe from (x0,y0) to (x1,y1)
	 *
	 *	@param (double) x0: x postion of pipe start
	 * 	@param (double) y0: y postion of pipe start
	 * 	@param (double) x1: x postion of pipe end
	 * 	@param (double) y1: y postion of pipe end
	 * 	@return (void)
	 */
	void add_pipe(double x0, double y0, double x1, double y1);

	/**
	 * Set beta learning controller option to _opt
	 *
	 * @param (bool) _opt: true, if beta learning on
	 * @return (void)
	 */
	void beta(bool _opt);

	Controller* c(int i=0);

	Environment* e();


	/**
	 * Set global vector learning controller option to _opt
	 *
	 * @param (bool) _opt: true, if GV learning on
	 * @return (void)
	 */
	void gvlearn(bool _opt);

	/**
	 * Set global vector navigation controller option to _opt (also changes gv_learn)
	 *
	 * @param (bool) _opt: true, if GV navigation on
	 * @return (void)
	 */
	void gvnav(bool _opt);

	/**
	 * Set local vector learning controller option to _opt
	 *
	 * @param (bool) _opt: true, if local vector learning on
	 * @return (void)
	 */
	void lvlearn(bool _opt);

	/**
	 * Set homing controller option to _opt
	 *
	 * @param (bool) _opt: true, if homing on
	 * @return (void)
	 */
	void homing(bool _opt);

	/**
	 * Initialize controller with parameters
	 *
	 * @param (int) num_neurons: number of neurons per layer (default: 18)
	 * @param (double) sensory_noise: sensory noise level (default: 0.0)
	 * @param (double) uncor_noise: uncorrelated noise level (default: 0.0)
	 * @param (double) leakage: leakage term for systematic errors (default: 0.0)
	 * @return (void)
	 */
	void init_controller(int num_neurons=18, double sensory_noise=0.0, double uncor_noise = 0.0, double leakage=0.0, double syn_noise=0.0);

	/**
	 * Reset simulation
	 *
	 * @return (void)
	 */
	void reset();

	/**
	 * Runs the simulation
	 *
	 *	@param (int) in_numtrials: number of trials
	 *	@param (double) in_duration: time duration of a trial
	 *	@param (double) in_interval: temporal integration interval
	 *	@return (void)
	 */
	void run(int in_numtrials, double in_duration, double in_interval);

	/**
	 * Set inward time step
	 *
	 * @param (int) _time: timesteps after agent goes inward
	 * @return (void)
	 */
	void set_inward(int _time);

	/**
	 * Updates simulation by one time step
	 *
	 * @return (void)
	 */
	void update();

	/**
	 * Writes global data into files (all trials)
	 *
	 * @return (void)
	 */
	void writeSimData();

	/**
	 * Writes trial data into files
	 *
	 * @return (void)
	 */
	void writeTrialData();


private:

	//************ Class objects ************//

	Environment* environment;
	vector<Controller*> controllers;

	//************ Simulation parameters ************//

	double T;						// maximum time for a trial
	int N;							// number of trials

	//************ Environment parameters ************//

	int agents = 1;                                   // number of agents
	const double m_rad = 40.;                         // maximum radius of objects
	const double g_dens = 0.01; //0.025;              // density of goals
	const int ngs = int(g_dens*(M_PI*pow(m_rad,2)));  // number of goals
	const double lm_dens = 0.05;                      // density of landmarks
	const int nlms = int(lm_dens*(M_PI*pow(m_rad,2)));// number of landmarks
	bool rand_env;                                    // option random environment

	//************ Output file streams ************//

	ofstream agent_str;
	ofstream endpts_str;
	ofstream error_dist;
	ofstream homevector_str;
	ofstream globalvector_str;
	ofstream localvector_str;
	ofstream refvector_str;
	ofstream reward_str;
	ofstream length_scaling;
	ofstream sim_cfg;
	ofstream out_signals;
	ofstream lmr_signals;
	ofstream lmr_angles;
	ofstream adaptive_expl;

	//************ Controller options *************//
	bool pin_on;			// true, if agent does PI
	bool homing_on;         // true, if agent does homing after certain time
	bool gvlearn_on;        // true, if agent learns global vectors
	bool gvnavi_on;         // true, if agent navigates global vectors
	bool lvlearn_on;		// true, if agent learns local vectors
	bool beta_on;		// true, if agent learns beta


	//************ Timing parameters ************//

	int trial;              // trial number
	double global_t;        // continuous time during multiple cycles
	double trial_t;         // continuous time for each trial
	double dt;              // integration time
	int timestep;           // discrete time steps
	int sample_time;		// how often data is written into file

public:
	//************ Evaluation parameters ************//

	/// Foraging
	running_stat<double> avg_length;

	/// Path integration
	running_stat<double> is_home;					// homing success
	running_stat<double> is_goal;					// goal success
	running_stat<double> avg_reward;				// goal success
	int count_home;
	int count_goal;
	running_stat<double> pi_error;              // distance between estimated and actual position of agent per timestep (reset each trial)
	running_stat<double> pi_error_max;              // distance between estimated and actual position of agent per timestep (reset each trial)
	running_stat<double> total_pi_error;		// distance between estimated and actual position of agent per timestep per trial
	//running_stat<double> avg_foraging_dis;

	//************ debugging ************//
	bool VERBOSE;
	bool SILENT;
};



#endif /* SIMULATION_H_ */
