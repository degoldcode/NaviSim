/*
 * goal.cpp
 *
 *  Created on: Jul 17, 2014
 *      Author: degoldschmidt
 */

#include <cmath>
#include <random>
#include <iostream>
#include <cstdio>
#include "goal.h"
using namespace std;

Goal::Goal(double max_radius){
	double min_radius = 1.;
	distance_to_origin = (max_radius-min_radius) * sqrt(rand(0.0, 1.0)) + min_radius;
	angle_to_x_axis = 2 * M_PI * rand(0.0, 1.0);
	amount = 1.0;//rand(0., 1.);
	amount_rate = 0.0;//0.0001;
	double prob_blue = 0.5;			// Probability of BLUE
	if(rand(0.,1.) < prob_blue){
		amount = 0.0;//1.0;
		goal_type = 0;
	}				// 0 = BLUE
	else{
		amount = 1.0;//0.25;
		goal_type = 1;
	}
	x_position = distance_to_origin * cos(angle_to_x_axis);
	y_position = distance_to_origin * sin(angle_to_x_axis);
	hit = 0;
	total_hits = 0;
}

Goal::Goal(double x, double y, int color){
	distance_to_origin = sqrt(x*x+y*y);
	angle_to_x_axis = atan2(y,x);
	amount_rate = 0.0;//0.0001;
	goal_type = color;
	printf("Goal added (%1.1f,%1.1f) with color %u", x, y, goal_type);
	if(goal_type==1)
		amount = 1.0;
	else
		amount = 0.25;
	printf("with amount = %f\n", amount);
	x_position = x;
	y_position = y;
	hit = 0;
	total_hits = 0;
}

Goal::~Goal(){

}

double Goal::a(){
	return amount;
}

double Goal::x(){
	return x_position;
}

double Goal::y(){
	return y_position;
}

double Goal::d(){
	return distance_to_origin;
}

double Goal::th(){
	return angle_to_x_axis;
}

int Goal::type(){
	return goal_type;
}

void Goal::swap(){
	amount = 1. - amount;
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
