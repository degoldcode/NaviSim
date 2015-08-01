/*****************************************************************************
 *  agent.cpp                                                                *
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

#include <cmath>
#include <random>
#include "agent.h"
using namespace std;

Agent::Agent(bool in_verbose, double x_0, double y_0) : Object(x_0, y_0, 0.0){
	VERBOSE = in_verbose;
	(VERBOSE)?printf("\nCREATE AGENT at (%g, %g)\n\n", x, y):VERBOSE;

	heading = rand(-M_PI, M_PI);		// random initial orientation
	speed = 0.1;

	k_phi = M_PI;
	k_s = 0.01;
	diff_heading = 0.0;
	external = 0.0;

	inward = false;
	in_pipe = false;
}

Agent::~Agent(){
}

Angle Agent::dphi(){
	return diff_heading;
}

double Agent::HV(int index){
	if(index==0)
		return control->HV()->x();
	if(index==1)
		return control->HV()->y();
	else
		return control->HV()->avg();
}

bool Agent::in(){
	return inward;
}

void Agent::init(Controller* control){
	this->control = control;
}

Angle Agent::phi(){
	return heading;
}

void Agent::reset(){
	x_position = 0.;
	y_position = 0.;
	heading = rand(-M_PI, M_PI);
	inward = false;
	//control->reset();
}

void Agent::set_dphi(double input){
	external = input;
}

void Agent::set_phi(double input){
	heading.setTo(input);
}

void Agent::set_inward(bool input){
	inward = input;
}

void Agent::to(double x_new, double y_new){
	x(x_new);
	y(y_new);
}

void Agent::update(){
	//control->set_inward(inward);
	control_output = control->update(heading.rad(), speed, 0.0, 0);
	if(!in_pipe)
		diff_heading = dt * k_phi * control_output + external;
	else
		diff_heading = external;
	external = 0.0;

	heading += diff_heading;

	diff_speed = dt * k_s * 0.0;
	speed += diff_speed;

	x_position += dt * speed * cos(heading);
	y_position += dt * speed * sin(heading);
}

double Agent::s(){
	return speed;
}
