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

Agent::Agent(){
	x = 0.;
	y = 0.;
	phi = rand(-M_PI, M_PI);
	v = 0.1;
	k_phi = 4.;
	m_dphi = 0.0;
	dphi = 0.0;
	abs_dphi = 0.0;
	distance = 0.0;
	theta = 0.0;
	no_write = false;
	stream.open("./data/agent.dat", ios_base::out /*| ios_base::app*/);
}

Agent::~Agent(){
	stream.close();
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

void Agent::update(double command){
	dphi = k_phi * command + m_dphi;
	phi += dphi;
	m_dphi = 0.0;
	phi = bound_angle(phi);
	abs_dphi = std::abs(dphi);
	x += v*cos(phi);
	y += v*sin(phi);
	distance = sqrt(x*x+y*y);
	theta = atan2(y,x);
	if(!no_write)
		stream << x << "\t" << y << "\t" << phi << "\t" << dphi << endl;
}

double Agent::rand(double min, double max){
	static random_device e{};
	static uniform_real_distribution<double> d(min, max);
	return d(e);
}
