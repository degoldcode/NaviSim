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
	t_step=0;
	VERBOSE = in_verbose;
	(VERBOSE)?printf("\nCREATE AGENT at (%g, %g)\n\n", x(), y()):VERBOSE;

	heading.to(randuu(-M_PI, M_PI));		// random initial orientation
	speed = 0.1;

	innate_lm_control = 0.0;
	diff_heading.to(0.0);
	external = new Angle(0.0);

	inward = false;
	in_pipe = false;
	lm_catch = false;
}

Agent::~Agent(){
	delete control;
}

Controller* Agent::c(){
	return control;
}

Angle Agent::dphi(){
	return diff_heading;
}

double Agent::get_lmcontrol(){
	return innate_lm_control;
}

Vec Agent::GV(int i){
	return control->GV(i);
}

Vec Agent::HV(){
	return control->HV();
}

Vec Agent::HVm(){
	return control->HVm();
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

PIN* Agent::pi(){
	return control->pi();
}

void Agent::reset(){
	to(0.,0.);
	heading.to(randuu(-M_PI, M_PI));
	inward = false;
	in_pipe = false;
	lm_catch = false;
	control->reset();
}

void Agent::set_dphi(Angle* input){
	external = input;
}

void Agent::set_lmcontrol(double input){
	innate_lm_control = input;
}

void Agent::set_phi(double input){
	heading.to(input);
}

void Agent::set_inward(bool input){
	inward = input;
}

void Agent::to(double x_new, double y_new){
	x(x_new);
	y(y_new);
}

void Agent::update(double _reward, vec _lmr){
	t_step++;
	//control->set_inward(inward);
	if(VERBOSE && t_step%100==0)
		printf("%u\n", lm_catch);
	if(!in_pipe && !lm_catch){
		if(VERBOSE && t_step%100==0)
			printf("Normal control: %f \n", control_output);
		diff_heading.to(dt * k_phi * control_output);
		heading = heading + diff_heading;
	}
	if(!in_pipe && lm_catch){
		if(VERBOSE && t_step%100==0)
			printf("LM control: %f \n", innate_lm_control);
		diff_heading.to(dt * k_phi * control_output + 1. * dt * k_phi * innate_lm_control);
		heading = heading + diff_heading; //+ dt *  0.5 * control_output;
	}
	if(in_pipe){
		if(VERBOSE && t_step%100==0)
			printf("Pipe control: %f \n", innate_lm_control);
		heading.to(external->rad());
	}

	diff_speed = dt * k_s * 0.0;
	speed += diff_speed;

	move(dt * speed * heading.C(), dt * speed * heading.S(), 0.0);

	control_output = control->update(heading.rad(), speed, _reward, _lmr, 0);
}

double Agent::s(){
	return speed;
}
