/*****************************************************************************
 *  landmark.h                                                               *
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

#ifndef LANDMARK_H_
#define LANDMARK_H_

#include "agent.h"
using namespace std;


/**
 * Landmark Class
 *
 * 	This class creates landmarks (randomly or specifically placed)
 * 	and handles landmark-related functions
 *
 */

class Landmark {
public:

	/**
	 * Constructor for randomly placed landmarks
	 *
	 *	@param (double) max_radius: maximum radius within the landmark is randomly placed
	 */
	Landmark(double max_radius);

	/**
	 * Constructor for specifically placed landmarks
	 *
	 * 	@param (double) x: x position of the landmark
	 * 	@param (double) y: y position of the landmark
	 */
	Landmark(double x, double y);

	/**
	 * Destructor
	 *
	 */
	~Landmark();


	//************ Class functions ************//

	/**
	 * Returns the distance of the landmark to the home position
	 *
	 * @return (double)
	 */
	double d();

	/**
	 * Returns the partially observable distance to landmark at the position (x,y)
	 *
	 *	@param (double) x: x position to be evaluated
	 *	@param (double) y: y position to be evaluated
	 * 	@return (double)
	 */
	double pod(double x, double y);

	/**
	 * Returns the partially observable distance to landmark at the position (x,y)
	 *
	 *	@param (Agent*) agent: agent to be evaluated
	 * 	@return (double)
	 */
	double pod(Agent* agent);

	/**
	 * Returns a random number drawn from a uniform distribution
	 *
	 * @param (double) min: lower bound of the distribution (default: 0.0)
	 * @param (double) max: upper bound of the distribution (default: 1.0)
	 * @return (double)
	 */
	double rand(double min, double max);

	/**
	 * Returns the angle of the vector from home position to the goal
	 *
	 * @return (double)
	 */
	double th();

	/**
	 * Returns total hits of the landmark
	 *
	 * @return (double)
	 */
	double total();

	/**
	 * Returns the x position of the landmark
	 *
	 * 	@return (double)
	 */
	double x();

	/**
	 * Returns the y position of the landmark
	 *
	 * 	@return (double)
	 */
	double y();

private:

	//************ Spatial parameters ************//
	double x_position;
	double y_position;
	double distance_to_origin;
	double angle_to_x_axis;
	const double landmark_radius = 0.2;			// radius for recognition

	//************ Analysis parameters ************//
	int total_hits = 0;				// total landmark hits of agents
};



#endif /* LANDMARK_H_ */
