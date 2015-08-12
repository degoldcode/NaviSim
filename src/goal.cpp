/*****************************************************************************
 *  goal.cpp                                                                 *
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
#include <iostream>
#include <cstdio>
#include "landmark.h"
#include "goal.h"
using namespace std;

enum{blue, yellow};						//colors

Goal::Goal(double max_radius, bool in_verbose, int color, double size, bool decay):
	Object(max_radius, 1.){
	VERBOSE = in_verbose;
	(VERBOSE)?printf("\nCREATE GOAL at (%g, %g)\n\n", pos.x, pos.y):VERBOSE;
	amount = size;                    //rand(0., 1.);
	if(decay)
		amount_rate = 0.0001;         // decay of reward loss
	else
		amount_rate = 0.0;
	//double prob_blue = 0.5;				// Probability of BLUE
	//bool s = sample(prob_blue);			// Sampling with probability for blue
	//amount = (s ? 0.0 : 1.0);
	goal_type = color;//(s ? 0 : 1);
}

Goal::Goal(double x, double y, bool in_verbose, int color, double size, bool decay):
	Object(x, y, 0.){
	VERBOSE = in_verbose;
	(VERBOSE)?printf("\nCREATE GOAL at (%g, %g)\n\n", pos.x, pos.y):VERBOSE;
	amount = size;
	if(decay)
		amount_rate = 0.001;//0.0001;         // decay of reward loss
	else
		amount_rate = 0.0;
	goal_type = color;
	//amount = (goal_type ? 1.0 : 0.25);
}

Goal::~Goal(){

}

double Goal::a(){
	return amount;
}

void Goal::a(const double value){
	amount = value;
}

int Goal::color(){
	return goal_type;
}

void Goal::color(const int value){
	goal_type = value;
}

void Goal::da(){
	amount -= amount_rate;
	if(amount<0.0)
		amount = 0.0;
}

double Goal::r(double x, double y, int mode){   //TODO move to environment
	if(d(x,y) < goal_radius && mode == 0){         //20 cm radius
		//factor = in_factor;
		amount -= amount_rate;
		//total_hits++;
		if(amount < 0.0){
			amount = 0.0;
			//hit = 0;
			//total_hits--;
		}
		//printf("Reward @ (%f, %f) = %f\n", x, y, amount);
		return (1./goal_radius)*(goal_radius-d(x,y))*amount*(1./d());
	}
	else
		return 0.0;
}

double Goal::rand(double min, double max){
	static random_device e{};
	static uniform_real_distribution<double> d(min, max);
	return d(e);
}

void Goal::reset(){

}

bool Goal::sample(double P){
	return (rand(0.,1.) < P);
}

void Goal::swap(){
	amount = 1. - amount;
}
