/*****************************************************************************
 *  goallearning.h                                                           *
 *                                                                           *
 *  Created on:   Jul 25, 2014                                               *
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

#ifndef GOALLEARNING_H_
#define GOALLEARNING_H_

#include "circulararray.h"
#include <armadillo>
using namespace arma;


/**
 * Goal Learning Circuit Class
 *
 * 	This class creates a circular array with
 * 	plastic connections for goal learning
 *
 */

class GoalLearning:
		public CircArray
{
public:
	/**
	 * Constructor
	 *
	 *  @param (int) num_neurons: number of neurons in this array (default: 360)
	 *  @param (int) input_dim: number of incoming signals (default: 0)
	 */
	GoalLearning(int num_neurons, double nnoise, bool opt_load=false);

	/**
	 * Destructor
	 *
	 */
	~GoalLearning();

	/**
	 * Get global vector
	 *
	 * @param (int) index: index of synaptic weights
	 * @return (Vec)
	 */
	Vec GV(int index = 0);

	/**
	 * Sets the foraging state
	 *
	 *	@param (double) state: foraging state
	 * 	@return (void)
	 */
	void set_mu(double state);

	/**
	 * Updates the goal learning circuit
	 *
	 *  @param (vec) pi_input: Input PI state
	 *  @param (double) in_reward: Reward signal
	 *  @param (double) in_expl: Exploration rate
	 *  @return (void)
	 */
	void update(vec pi_input, double in_reward, double in_expl);

	/**
	 * Updates the path integration network
	 *
	 *  @param (double) angle: Input angle from compass
	 *  @param (double) speed: Input walking speed from odometry (legged: "differential step counter")
	 *  @return (void)
	 */
	void update_weights(vec pi_input);

	/**
	 * Returns the GV x coordinate
	 *
	 * @return (double)
	 */
	double x();

	/**
	 * Returns the GV y coordinate
	 *
	 * @return (double)
	 */
	double y();

private:
	vector<Vec> global_vector;                   // Global vectors

	double foraging_state;
	double learn_rate;
	double reward;
	double expl_rate;

	mat weight_change;

	bool load_weights;
};



#endif /* GOALLEARNING_H_ */
