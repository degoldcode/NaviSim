/*
 * goal.cpp
 *
 *  Created on: Jul 17, 2014
 *      Author: degoldschmidt
 */

#include <cmath>
#include <random>
#include <iostream>
#include "goal.h"
using namespace std;

Goal::Goal(double max_radius){
	double min_radius = 2.;
	distance_to_origin = (max_radius-min_radius) * sqrt(rand(0.0, 1.0)) + min_radius;
	angle_to_x_axis = 2 * M_PI * rand(0.0, 1.0);

	x_position = distance_to_origin * cos(angle_to_x_axis);
	y_position = distance_to_origin * sin(angle_to_x_axis);
	hit = 0;
	thit = 0;
}

Goal::Goal(double x, double y){
	distance_to_origin = sqrt(x*x+y*y);
	angle_to_x_axis = atan2(y,x);

	x_position = x;
	y_position = y;
	hit = 0;
	thit = 0;
}

Goal::~Goal(){

}

double Goal::get_reward(double x, double y){
	double rxsqr = pow(x-x_position, 2.);
	double rysqr = pow(y-y_position, 2.);
	double rdist = sqrt(rxsqr+rysqr);
	if(rdist < 0.2){ //20 cm radius
		hit = 1;
		thit++;
		cout << "Hit\n";
		return 1.0;
	}
	else
		return 0.0;
}

double Goal::rand(double min, double max){
	static random_device e{};
	static uniform_real_distribution<double> d(min, max);
	return d(e);
}
