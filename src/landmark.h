/*****************************************************************************
 *  landmark.h                                                               *
 *                                                                           *
 *  Created on:   May 18, 2014                                               *
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

#include "object.h"
using namespace std;

/**
 * Landmark Class
 *
 * 	This class creates landmarks (randomly or specifically placed)
 * 	and handles landmark-related functions
 *
 */

class Landmark : public Object {
public:
	/**
		 * Constructor for randomly placed landmarks
		 *
		 *	@param (double) max_radius: maximum radius within the landmark is randomly placed
		 *	@param (bool) in_verbose: debugging (default: false)
		 */
		Landmark(double max_radius, bool in_verbose = false);

		/**
		 * Constructor for specifically placed landmarks
		 *
		 * 	@param (double) x: x position of the landmark
		 * 	@param (double) y: y position of the landmark
		 * 	@param (bool) in_verbose: debugging (default: false)
		 */
		Landmark(double x, double y, bool in_verbose = false);

		/**
		 * Destructor
		 *
		 */
		~Landmark();


		//************ Class functions ************//

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

private:
		const double lm_radius = 0.2;
		bool VERBOSE;
};


#endif /* LANDMARK_H_ */
