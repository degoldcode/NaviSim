/*
 * agent.h
 *
 *  Created on: Jul 17, 2014
 *      Author: degoldschmidt
 */

#ifndef AGENT_H_
#define AGENT_H_

#include <iostream>
#include <fstream>
using namespace std;

class Agent {
public:
	Agent(int mySampling);
	~Agent();

	double bound_angle(double angle);
	void displace_to(double x_new, double y_new);
	double rand(double min, double max);
	void reset();
	void update(double command);

	double x;
	double y;
	double phi;
	double v;
	double k_phi;
	double dphi;
	double abs_dphi;
	double theta;
	double distance;

	//timing
	double t;
	const double dt = 0.1;
	int ts;
	int trial;

	ofstream stream;
	int sampling_interval;

	bool in_pipe;
};



#endif /* WALKER_H_ */
