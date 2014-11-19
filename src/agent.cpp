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

Agent::Agent(double x, double y, bool in_verbose){
	verbose = in_verbose;
	if(verbose) cout << "Create AGENT" << endl;

	x_position = x;
	y_position = y;
	heading = rand(-M_PI, M_PI);		// random initial orientation
	speed = 0.1;

	k_phi = M_PI;
	k_s = 0.01;
	diff_heading = 0.0;
	external = 0.0;
	distance = 0.0;
	theta = 0.0;

	in_pipe = false;
}

Agent::~Agent(){
}

double Agent::bound(double angle){
	double rphi = angle;
	while(rphi > M_PI)
		rphi -= 2 * M_PI;
	while(rphi < - M_PI)
		rphi += 2 * M_PI;
	return rphi;
}

double Agent::d(){
	return distance;
}

double Agent::dphi(){
	return diff_heading;
}

double Agent::phi(){
	return heading;
}

double Agent::rand(double min, double max){
	static random_device e{};
	static uniform_real_distribution<double> d(min, max);
	return d(e);
}

void Agent::reset(){
	x_position = 0.;
	y_position = 0.;
	heading = rand(-M_PI, M_PI);
}

void Agent::set_dphi(double input){
	external = input;
}

void Agent::set_phi(double input){
	heading = input;
}

void Agent::set_type(int input){
	type = input;
}

void Agent::set_x(double input){
	x_position = input;
}

void Agent::set_y(double input){
	y_position = input;
}

double Agent::th(){
	return theta;
}

void Agent::to(double x_new, double y_new){
	x_position = x_new;
	y_position = y_new;
}

void Agent::update(double command, double speed_command){
	if(!in_pipe)
		diff_heading = dt * k_phi * command + external;
	else
		diff_heading = external;
	external = 0.0;

	heading += diff_heading;
	heading = bound(heading);

	diff_speed = dt * k_s * speed_command;
	speed += diff_speed;

	x_position += dt * speed * cos(heading);
	y_position += dt * speed * sin(heading);
	distance = sqrt(pow(x_position,2)+pow(y_position,2));
	theta = bound(atan2(y_position,x_position));
}

double Agent::v(){
	return speed;
}

double Agent::x(){
	return x_position;
}

double Agent::y(){
	return y_position;
}