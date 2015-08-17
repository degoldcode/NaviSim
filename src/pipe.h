/*****************************************************************************
 *  pipe.h                                                                   *
 *                                                                           *
 *  Created on:   Aug 02, 2014                                               *
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

#ifndef PIPE_H_
#define PIPE_H_

#include "agent.h"
using namespace std;


/**
 * Pipe Class
 *
 * 	This class creates pipes (or channel), in
 * 	which the agent keeps a fixed orientation
 * 	in the direction of the pipe
 *
 */

class Pipe {
public:

	/**
	 * Constructor
	 *
	 *	@param (double) x0: x position of start
	 *	@param (double) x1: x position of end
	 *	@param (double) y0: y position of start
	 *	@param (double) y1: y position of end
	 *
	 */
	Pipe(double x0, double x1, double y0, double y1);

	/**
	 * Destructor
	 *
	 */
	~Pipe();


	//************ Class functions ************//

	/**
	 * Returns agent pointer setting the agent into pipe
	 *
	 * @param (Agent*): agent to be processed
	 * @return (Agent*)
	 *
	 */
	//Agent* set_agent_pipe(Agent* agent);

	Object* in();

	Object* out();

	Angle phi();

	/**
	 * Returns width of the pipee
	 *
	 *	@return (double)
	 *
	 */
	double w();

	/**
	 * Returns x position of the start of the pipe
	 *
	 *	@return (double)
	 *
	 */
	double x0();

	/**
	 * Returns x position of the end of the pipee
	 *
	 *	@return (double)
	 *
	 */
	double x1();

	/**
	 * Returns a coordinate-transformed x position in the
	 * reference system of the pipe
	 *
	 * 	@param (double) x: x position
	 *	@param (double) y: y position
	 *	@return (double)
	 *
	 */
	double x_pipe(double x, double y);

	/**
	 * Returns y position of the start of the pipe
	 *
	 *	@return (double)
	 *
	 */
	double y0();

	/**
	 * Returns y position of the end of the pipe
	 *
	 *	@return (double)
	 *
	 */
	double y1();

	/**
	 * Returns a coordinate-transformed y position in the
	 * reference system of the pipe
	 *
	 * 	@param (double) x: x position
	 *	@param (double) y: y position
	 *	@return (double)
	 *
	 */
	double y_pipe(double x, double y);

	Angle alpha;				// angle of pipe w.r.t. to x-axis

private:

	//************ Spatial parameters ************//
	Object* entry;				// position of start
	Object* exit;				// position of end

	const double p_width = 0.05;// width of pipe
	double len;					// length of pipe

	//************ Other parameters ************//
	bool in_this_pipe = false;	// bool (TRUE = agent is in this pipe, FALSE = if not)
};

#endif /* PIPE_H_ */
