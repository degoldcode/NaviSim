/*****************************************************************************
 *  landmark.cpp                                                             *
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

#include <cmath>
#include <random>
#include <iostream>
#include <cstdio>
#include "landmark.h"
using namespace std;


Landmark::Landmark(double max_radius, bool in_verbose):
	Object(max_radius, 0.5){
	VERBOSE = in_verbose;
	(VERBOSE)?printf("\nCREATE LANDMARK at (%g, %g)\n\n", pos.x, pos.y):VERBOSE;
}

Landmark::Landmark(double x, double y, bool in_verbose):
	Object(x, y, 0.){
	VERBOSE = in_verbose;
	(VERBOSE)?printf("\nCREATE LANDMARK at (%g, %g)\n\n", pos.x, pos.y):VERBOSE;
}

Landmark::~Landmark(){

}

double Landmark::rand(double min, double max){
	static random_device e{};
	static uniform_real_distribution<double> d(min, max);
	return d(e);
}

void Landmark::reset(){

}

