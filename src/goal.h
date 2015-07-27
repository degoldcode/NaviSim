/*****************************************************************************
 *  goal.h                                                                   *
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

#ifndef GOAL_H_
#define GOAL_H_

#include "object.h"
using namespace std;


/**
 * Goal Class
 *
 * 	This class creates goals (randomly or specifically placed)
 * 	and handles goal-related functions
 *
 */

class Goal : public Object {
public:

	/**
	 * Constructor for randomly placed goals
	 *
	 *	@param (double) max_radius: maximum radius within the goal is randomly placed
	 *	@param (bool) in_verbose: debugging (default: false)
	 *	@param (int) color: color index of the goal (default: 0)
	 *	@param (double) size: reward size of goal (default: 1.)
	 *	@param (bool) decay: indicates, whether goal reward decays with every visit
	 */
	Goal(double max_radius, bool in_verbose = false, int color = 0, double size = 1., bool decay=false);

	/**
	 * Constructor for specifically placed goals
	 *
	 * 	@param (double) x: x position of the goal
	 * 	@param (double) y: y position of the goal
	 * 	@param (bool) in_verbose: debugging (default: false)
	 * 	@param (int) color: color index of the goal (default: 0)
	 * 	@param (double) size: reward size of goal (default: 1.)
	 *	@param (bool) decay: indicates, whether goal reward decays with every visit
	 */
	Goal(double x, double y, bool in_verbose = false, int color = 0, double size = 1., bool decay=false);

	/**
	 * Destructor
	 *
	 */
	~Goal();


	//************ Class functions ************//

	/**
	 * Returns the amount of reward stored at the goal
	 *
	 *	@return (double)
	 */
	double a();

	/**
	 * Sets the amount of reward stored at the goal
	 *
	 *	@param (double) value: new reward size of goal
	 */
	void a(const double value);

	/**
	 * Returns the goal type (color index) of the goal
	 *
	 * @return (int)
	 */
	int color();

	/**
	 * Sets the goal type (color index) of the goal
	 *
	 * @param (int) value: new color index of the goal
	 */
	void color(const int value);

	/**
	 * Returns the reward given at the position (x,y) given the foraging state mode
	 *
	 *	@param (double) x: x position to be evaluated
	 *	@param (double) y: y position to be evaluated
	 *	@param (int) mode: foraging state
	 * 	@return (double)
	 */
	double r(double x, double y, int mode);

	/**
	 * Returns a random number drawn from a uniform distribution
	 *
	 * @param (double) min: lower bound of the distribution (default: 0.0)
	 * @param (double) max: upper bound of the distribution (default: 1.0)
	 * @return (double)
	 */
	double rand(double min, double max);

	/**
	 * Resets trial-based goal statistics
	 *
	 * @return (void)
	 */
	void reset();

	/**
	 * Returns bool if random sample is of a type with probability P
	 *
	 * @param (double) P: probability for sample to be of given type
	 * @return (bool)
	 */
	bool sample(double P);

	/**
	 * Swaps the reward given for this goal r->(1-r)
	 *
	 * @return (void)
	 */
	void swap();

private:
	//************ Reward parameters ************//
	double amount;				// amount of reward
	double amount_rate;			// rate of reward loss
	int goal_type;				// color
	const double goal_radius = 0.2;

	//************ Debugging ************//
	bool VERBOSE;
};



#endif /* GOAL_H_ */
