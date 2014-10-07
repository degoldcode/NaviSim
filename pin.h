/*****************************************************************************
 *	environment.h															 *
 *																			 *
 *  Created on:		Jul 23, 2014											 *
 *  Author: 		Dennis Goldschmidt										 *
 *  Email: 			goldschmidtd@ini.phys.ethz.ch							 *
 *  								 										 *
 *  								 										 *
 *  Copyright (C) 2014 by Dennis Goldschmidt								 *
 *  																		 *
 *  This file is part of the program NaviSim                                 *
 *                                                                           *
 *  NaviSim is free software: you can redistribute it and/or modify     	 *
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

#ifndef PIN_H_
#define PIN_H_

#include <armadillo>
using namespace std;
using namespace arma;

class PIN {
public:
	PIN(int neurons, double leak, double sensnoise, double neurnoise);
	~PIN();

	void reset();
	vec update(double angle, double speed);
	vec lin_rect(vec input);
	double get_avg_angle(vec input);
	double get_max_angle(vec input);
	double get_max_value(vec input);
	double gaussian_noise(double width);
	double bound_PI_angle(double angle);
	double maxeigenvalue(mat & A);

	vec pref_angle;
	vec act_head_direction;
	vec act_gater;
	vec act_integrator;
	vec act_output;
	mat w_cos;

	double max_angle;
	double avg_angle;
	double length;
	double memory_length;

	int N;
	double leak_rate;
	double snoise;
	double nnoise;
	int t;
};



#endif /* PIN_H_ */
