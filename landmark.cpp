/*****************************************************************************
 *  landmark.cpp                                                             *
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
#include "landmark.h"
using namespace std;

Landmark::Landmark(double max_radius){
	double min_radius = .5;					//goals are at least .5m away from home
	distance_to_origin = (max_radius-min_radius) * sqrt(rand(0.0, 1.0)) + min_radius;
	angle_to_x_axis = 2 * M_PI * rand(0.0, 1.0);

	x_position = distance_to_origin * cos(angle_to_x_axis);
	y_position = distance_to_origin * sin(angle_to_x_axis);
}

Landmark::Landmark(double x, double y){
	distance_to_origin = sqrt(x*x+y*y);
	angle_to_x_axis = atan2(y,x);

	x_position = x;
	y_position = y;
}

Landmark::~Landmark(){

}

double Landmark::d(){
	return distance_to_origin;
}

double Landmark::pod(double x, double y){
	double rxsqr = pow(x-x_position, 2.);
	double rysqr = pow(y-y_position, 2.);
	double rdist = sqrt(rxsqr+rysqr);
	if(rdist < landmark_radius){ 				//20 cm radius
		total_hits++;
		return 1.0;//2.0*(0.2-rdist);
	}
	return 0.0;
}

double Landmark::pod(Agent* agent){
	double rxsqr = pow(agent->x()-x_position, 2.);
	double rysqr = pow(agent->y()-y_position, 2.);
	double rdist = sqrt(rxsqr+rysqr);
	if(rdist < landmark_radius){ 				//20 cm radius
		total_hits++;
		return 1.0;//2.0*(0.2-rdist);
	}
	return 0.0;
}

double Landmark::rand(double min, double max){
	static random_device e{};
	static uniform_real_distribution<double> d(min, max);
	return d(e);
}


double Landmark::th(){
	return angle_to_x_axis;
}

double Landmark::total(){
	return total_hits;
}

double Landmark::x(){
	return x_position;
}

double Landmark::y(){
	return y_position;
}
