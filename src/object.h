/*****************************************************************************
 *  object.h                                                                 *
 *                                                                           *
 *  Created on:   May 11, 2015                                               *
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

#ifndef OBJECT_H_
#define OBJECT_H_

#include <random>
#include "geom.h"

/**
 * Abstract Object Class
 *
 * 	This class creates objects (randomly or specifically placed)
 * 	and handles object-related functions
 *
 */

class Object {
public:

	/**
	 * Constructor for randomly placed objects
	 *
	 *	@param (double) max_radius: maximum radius within the object is randomly placed
	 *	@param (double) min_radius: minimum radius within the object is randomly placed
	 */
	Object(double max_radius, double min_radius=0.);

	/**
	 * Constructor for specifically placed object
	 *
	 * 	@param (double) _x: x position of the object
	 * 	@param (double) _y: y position of the object
	 * 	@param (double) _z: z position of the object
	 */
	Object(double _x, double _y, double _z);

	/**
	 * Destructor
	 *
	 */
	~Object();


	//************ Class functions ************//

	/**
	 * Return distance of the object to the origin
	 *
	 * @return (double)
	 */
	double d();

	/**
	 * Return distance of the object to given position
	 *
	 * @param (double) _x: x position to be evaluated
	 * @param (double) _y: y position to be evaluated
	 * @param (double) _z: z position to be evaluated
	 * @return (double)
	 */
	double d(double _x, double _y, double _z = 0.);

	/**
	 * Move position of the object by (_dx, _dy, _dz)
	 *
	 * @param (double) _dx: x disposition of object
	 * @param (double) _dy: y disposition of object
	 * @param (double) _dz: z disposition of object (default: 0.)
	 * @return (void)
	 */
	void move(double _dx, double _dy, double _dz = 0.0);

	/**
	 * Return random number drawn from a uniform distribution
	 *
	 * @param (double) min: lower bound of the distribution (default: 0.0)
	 * @param (double) max: upper bound of the distribution (default: 1.0)
	 * @return (double)
	 */
	double randuu(double min, double max);

	/**
	 * Return azimuthal angle of the vector from origin to the object
	 *
	 * @return (double)
	 */
	Angle th();

	/**
	 * Return azimuthal angle of the vector from the object to a given position
	 *
	 * @return (double)
	 */
	Angle th(double _x, double _y);

	/**
	 * Move position of the object to (_x, _y, _z)
	 *
	 * @param (double) _x: new x position of object
	 * @param (double) _y: new y position of object
	 * @param (double) _z: new z position of object (default: 0.)
	 * @return (void)
	 */
	void to(double _x, double _y, double _z = 0.0);

	/**
	 * Return vector of the object
	 *
	 * @return (Vec)
	 */
	Vec v();

	/**
	 * Return x position of the object
	 *
	 * 	@return (double)
	 */
	double x();

	/**
	 * Set x position of the object to _x
	 *
	 * @param (double) _x: new x position of object
	 * @return (double)
	 */
	void x(double _x);

	/**
	 * Return y position of the object
	 *
	 * @return (double)
	 */
	double y();

	/**
	 * Set y position of the object to _y
	 *
	 * @param (double) _y: new y position of object
	 * @return (double)
	 */
	void y(double _y);

	/**
	 * Return z position of the object
	 *
	 * @return (double)
	 */
	double z();

	/**
	 * Set z position of the object to _z
	 *
	 * @param (double) _z: new z position of object
	 * @return (double)
	 */
	void z(double _z);

	//************ Spatial parameters ************//
	Vec pos;

};


#endif /* OBJECT_H_ */
