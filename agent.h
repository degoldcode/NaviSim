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
	double get_x();
	double get_y();
	double rand(double min, double max);
	void reset();
	void update(double command);

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
	ofstream stream_s;
	int sampling_interval;

	bool short_write;
	bool in_pipe;

private:
	double x;
	double y;
};



#endif /* WALKER_H_ */
