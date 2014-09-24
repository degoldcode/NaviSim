/*
 * walker.cpp
 *
 *  Created on: Jul 17, 2014
 *      Author: degoldschmidt
 */

#include <cmath>
#include <random>
#include "agent.h"
using namespace std;

Agent::Agent(int mySampling){
	x = 0.;
	y = 0.;
	phi =  rand(-M_PI, M_PI);
	v = 0.1;
	k_phi = M_PI;
	dphi = 0.0;
	abs_dphi = 0.0;
	distance = 0.0;
	theta = 0.0;
	sampling_interval = mySampling;
	in_pipe = false;
	short_write = true;

	t = 0.0;
	trial_t = 0.0;
	ts = 0;
	trial = 1;

	stream.open("./data/agent.dat", ios_base::out /*| ios_base::app*/);
	stream_s.open("./data/agent2.dat", ios_base::out /*| ios_base::app*/);
}

Agent::~Agent(){
	stream.close();
	stream_s.close();
}

double Agent::bound_angle(double phi){
	double rphi;
	rphi = phi;
	while(rphi > M_PI)
		rphi -= 2 * M_PI;
	while(rphi < - M_PI)
		rphi += 2 * M_PI;
	return rphi;
}

void Agent::displace_to(double x_new, double y_new){
	x = x_new;
	y = y_new;
}

double Agent::get_x(){
	return x;
}

double Agent::get_y(){
	return y;
}

double Agent::rand(double min, double max){
	static random_device e{};
	static uniform_real_distribution<double> d(min, max);
	return d(e);
}

void Agent::reset(){
	x = 0.;
	y = 0.;
	trial_t = 0.0;
	phi = rand(-M_PI, M_PI);
	trial++;
}

void Agent::update(double command){
	//if(ts%sampling_interval==0)
		stream  << t << "\t"
				<< x << "\t"
				<< y << "\t"
				<< phi << "\t"
				<< dphi << "\t"
				<< theta << "\t"
				<< distance << "\t"
				<< trial  << "\t"
				<< trial_t<< endl;
		if(short_write)
			stream_s<< t << "\t"
					<< x << "\t"
					<< y << "\t"
					<< trial  << endl;


	if(!in_pipe)
		dphi = dt * k_phi * command;
	phi += dphi;
	phi = bound_angle(phi);
	abs_dphi = abs(dphi);
	x += v*cos(phi);
	y += v*sin(phi);
	distance = sqrt(x*x+y*y);
	theta = bound_angle(atan2(y,x));

	ts++;
	trial_t+=dt;
	t=ts*dt;
}
