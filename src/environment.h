/*****************************************************************************
 *  environment.h                                                            *
 *                                                                           *
 *  Created on:   Jul 17, 2014                                               *
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

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "agent.h"
#include "goal.h"
#include "landmark.h"
//#include "pipe.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

/*** Foraging states ***/
enum{outbound, inbound};


/**
 * Environment Class
 *
 * 	This class handles objects and agents in the
 * 	environment and provides function and parameters
 * 	for simulation
 *
 */

class Environment {
public:

	/**
	 * Constructor for an empty environment
	 *
	 *	@param (int) num_agents: number of agents in this environment (default: 1)
	 *
	 */
	Environment(int num_agents=1);

	/**
	 * Constructor for an environment with randomly distributed goals and landmarks
	 *
	 *	@param (int) num_goals: number of goals in environment
	 *	@param (int) num_landmarks: number of landmarks in enviornment
	 *	@param (double) max_radius: maximum radius of objects in environment
	 *	@param (int) num_agents: number of agents in this environment (default: 1)
	 *
	 */
	Environment(int num_goals, int num_landmarks, double max_radius, int num_agents=1);

	/**
	 * Destructor
	 *
	 */
	~Environment();


	//************ Class functions ************//

	/**
	 * Returns agent pointer with given index
	 *
	 *	@param (int) i: index of agent (default: 0)
	 *	@return (Agent*)
	 */
	Agent* a(int i=0);

	/**
	 * Adds an agent at position (x,y)
	 *
	 *	@param (double) x: x position of agent (default: 0)
	 *	@param (double) y: y position of agent (default: 0)
	 *	@return (void)
	 */
	void a(double x=0., double y=0.);

	/**
	 * Adds an agent at position (x,y)
	 *
	 *	@param (double) x: x position of agent
	 *	@param (double) y: y position of agent
	 *	@return (void)
	 */
	void add_agent(double x, double y);

	/**
	 * Adds a goal at position (x,y) with color
	 *
	 * 	@param (double) x: x postion of goal
	 * 	@param (double) y: y postion of goal
	 * 	@param (int) color: color index of goal (default: 0)
	 * 	@return (void)
	 */
	void add_goal(double x, double y, int color = 0);

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
	 * Adds a randomly placed landmark
	 *
	 *	@param (double) max_radius: maximum radius of placement
	 * 	@return (void)
	 */
	void add_landmark(double max_radius);

	/**
	 * Adds a pipe from (x0,y0) to (x1,y1)
	 *
	 *	@param (double) x0: x postion of pipe start
	 * 	@param (double) y0: y postion of pipe start
	 * 	@param (double) x1: x postion of pipe end
	 * 	@param (double) y1: y postion of pipe end
	 * 	@return (void)
	 */
	//void add_pipe(double x0, double y0, double x1, double y1);

	/**
	 * Returns color index of nearest goal
	 *
	 *	@return (int)
	 */
	//int color();

	/**
	 * Returns the distance between two given objects
	 *
	 * 	@param (Object*) g1: first object
	 * 	@param (Object*) g2: second object
	 * 	@return (double)
	 */
	double d(Object* o1, Object* o2);

	/**
	 * Returns the distance between two points x0, x1 in 2D plane
	 *
	 * 	@param (Goal*) x0: first point
	 * 	@param (Goal*) x1: second point
	 * 	@return (double)
	 */
	double d(double x0, double x1);

	/**
	 * Returns number of hits of all goals
	 *
	 * 	@return (int)
	 */
	//int get_hits();

	/**
	 * Returns number of hits of goal i
	 *
	 *	@param (int) i: goal index
	 * 	@return (int)
	 */
	//int get_hits(int i);

	/**
	 * Returns sum of reward given during the trial
	 *
	 * 	@return (double)
	 */
	//double get_sum_reward();

	/**
	 * Returns total amount of reward given
	 *
	 * 	@return (double)
	 */
	//double get_total_reward();

	/**
	 * Returns goal pointer with given index
	 *
	 *	@param (int) i: index of goal (default: 0)
	 *	@return (Goal*)
	 */
	//Goal* g(int i=0);

	/**
	 * Returns landmark recognition signal
	 *
	 * 	@return (double)
	 */
	//double lmr();

	/**
	 * Returns number of goals
	 *
	 * 	@return (int)
	 */
	//int n_goals();

	/**
	 * Returns goal pointer of the nearest goal from a given position
	 *
	 *	@param (double) x: position to be evaluated (default: 0.)
	 *	@param (double) y: position to be evaluated (default: 0.)
	 *	@return (Goal*)
	 */
	//Goal* nearest(double x=0.0, double y=0.0);

	/**
	 * Returns reward signal
	 *
	 * 	@return (double)
	 */
	//double r();

	/**
	 * Resets the environment -> reset of agent and trial-based statistics
	 *
	 *	@return (void)
	 */
	void reset();

	/**
	 * Sets the foraging state
	 *
	 *	@param in_mode: foraging state
	 * 	@return (void)
	 */
	void set_mode(int in_mode);

	/**
	 * Swaps the reward ratio of the goals to r->(1-r)
	 *
	 * 	@return (void)
	 */
	//void swap_reward();

	/**
	 * Returns the "real" HV angle of agent i
	 *
	 *	@param (int) i: agent index (default: 0)
	 * 	@return (double)
	 */
	double th(int i=0);

	/**
	 * Updates the environment with given motor command
	 *
	 *	@return (void)
	 */
	void update();

	/**
	 * Updates agents with given motor command
	 *
	 *	@return (void)
	 */
	void update_agents();

	/**
	 * Returns x position of agent i
	 *
	 *	@param (int) i: agent index (default: 0)
	 * 	@return (double)
	 */
	double x(int i=0);

	/**
	 * Returns y position of agent i
	 *
	 *	@param (int) i: agent index (default: 0)
	 * 	@return (double)
	 */
	double y(int i=0);


private:

	//************ Reward parameters ************//
	double reward = 0.0;
	double sum_reward = 0.0;			// sum of reward during trial
	double total_reward = 0.0;			// total reward of cycle
	double lm_recogn = 0.0;
	bool flag = 0;
	int count = 0;
	int mode = 0; 						// 0 = outb, 1 = inb
	int inv_sampling_rate;

	//************ Object containers ************//
	vector<Agent*> agent_list;
	vector<Goal*> goal_list;
	vector<Landmark*> landmark_list;
	//vector<Pipe*> pipe_list;

	//************ output file streams ************//
	ofstream stream_g;		//goal positions
	ofstream stream_lm;		//landmark positions
	ofstream stream_p;		//pipe positions
	ofstream stream_h;		//home positions
	ofstream stream_food;	//amount of food at goals over time

	//************ debugging ************//
	const bool VERBOSE = false;
};

#endif /* ENVIRONMENT_H_ */
