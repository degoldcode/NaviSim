/*
 * landmark.cpp
 *
 *  Created on: Jul 17, 2014
 *      Author: degoldschmidt
 */

#include <cmath>
#include <random>
#include "landmark.h"
using namespace std;

Landmark::Landmark(double max_radius){
	double min_radius = .5;
	distance_to_origin = (max_radius-min_radius) * sqrt(rand(0.0, 1.0)) + min_radius;
	angle_to_x_axis = 2 * M_PI * rand(0.0, 1.0);

	x_position = distance_to_origin * cos(angle_to_x_axis);
	y_position = distance_to_origin * sin(angle_to_x_axis);
	total_hits = 0;
}

Landmark::Landmark(double x, double y){
	distance_to_origin = sqrt(x*x+y*y);
	angle_to_x_axis = atan2(y,x);

	x_position = x;
	y_position = y;
	total_hits = 0;
}

Landmark::~Landmark(){

}

double Landmark::x(){
	return x_position;
}

double Landmark::y(){
	return y_position;
}

double Landmark::d(){
	return distance_to_origin;
}

double Landmark::th(){
	return angle_to_x_axis;
}


double Landmark::get_hit(double x, double y){
	double rxsqr = pow(x-x_position, 2.);
	double rysqr = pow(y-y_position, 2.);
	double rdist = sqrt(rxsqr+rysqr);
	if(rdist < 0.2){ 	//50 cm radius
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
