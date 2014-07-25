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
	Agent();
	~Agent();

	double bound_angle(double angle);
	void update(double command);
	double rand(double min, double max);

	double x;
	double y;
	double phi;
	double v;
	double k_phi;
	double dphi;
	double m_dphi;
	double abs_dphi;
	double theta;
	double distance;

	ofstream stream;
	bool no_write;
};



#endif /* WALKER_H_ */
