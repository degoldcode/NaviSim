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
#include "goal.h"
using namespace std;

enum{blue, yellow};						//colors

Goal::Goal(double max_radius){
	double min_radius = 1.;				//goals are at least 1m away from home
	distance_to_origin = (max_radius-min_radius) * sqrt(rand(0.0, 1.0)) + min_radius;
	angle_to_x_axis = 2 * M_PI * rand(0.0, 1.0);
	amount = 1.0;						//rand(0., 1.);
	amount_rate = 0.0;					//0.0001;// decay of reward loss
	//double prob_blue = 0.5;				// Probability of BLUE
	//bool s = sample(prob_blue);			// Sampling with probability for blue
	//amount = (s ? 0.0 : 1.0);
	goal_type = 0;//(s ? 0 : 1);

	x_position = distance_to_origin * cos(angle_to_x_axis);
	y_position = distance_to_origin * sin(angle_to_x_axis);
}

Goal::Goal(double x, double y, int color){
	x_position = x;
	y_position = y;
	distance_to_origin = sqrt(x*x+y*y);
	angle_to_x_axis = atan2(y,x);
	amount_rate = 0.0;//0.0001;
	goal_type = color;
	printf("Goal added (%1.1f,%1.1f) with color %u ", x, y, goal_type);
	amount = (goal_type ? 1.0 : 0.25);
	printf("and amount = %f\n", amount);
}

Goal::~Goal(){

}

double Goal::a(){
	return amount;
}

int Goal::color(){
	return goal_type;
}

double Goal::d(){
	return distance_to_origin;
}

int Goal::hits(){
	return hit;
}

double Goal::r(double x, double y, int mode){
	double rxsqr = pow(x-x_position, 2.);
	double rysqr = pow(y-y_position, 2.);
	double rdist = sqrt(rxsqr+rysqr);
	if(rdist < 0.2 && mode == 0){ //20 cm radius
		hit = 1;
		//factor = in_factor;
		amount -= amount_rate;
		total_hits++;
		if(amount < 0.0){
			amount = 0.0;
			hit = 0;
			total_hits--;
		}
		//printf("Reward @ (%f, %f) = %f\n", x, y, amount);
		return 5.*(0.2-rdist)*amount*(1./distance_to_origin);
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
	hit = 0;
}

bool Goal::sample(double P){
	return (rand(0.,1.) < P);
}

void Goal::swap(){
	amount = 1. - amount;
}

double Goal::th(){
	return angle_to_x_axis;
}

int Goal::total(){
	return total_hits;
}

double Goal::x(){
	return x_position;
}

double Goal::y(){
	return y_position;
}
