/*
 * pipe.cpp
 *
 *  Created on: 02.08.2014
 *      Author: meicron
 */


#include <cmath>
#include "pipe.h"
using namespace std;

Pipe::Pipe(double x0, double x1, double y0, double y1, double width){
	x_pos_0 = x0;
	x_pos_1 = x1;
	y_pos_0 = y0;
	y_pos_1 = y1;
	p_width = width;
	double dx = x1 - x0;
	double dy = y1 - y0;
	alpha = atan2(dy, dx);
	len = sqrt(dx*dx + dy*dy);
	in_this_pipe = false;
	//cout << alpha << " " << len << endl;
}

Pipe::~Pipe(){

}

Agent* Pipe::set_agent_pipe(Agent* agent){
	double x_m = get_x_pipe(agent->get_x(), agent->get_y());
	double y_m = get_y_pipe(agent->get_x(), agent->get_y());
	if((x_m > -p_width && x_m < p_width && abs(y_m) < .5*p_width) || in_this_pipe){
		agent->dphi = alpha - agent->phi;
		in_this_pipe = true;
		//cout << x_m << " " << y_m << " " << agent->phi << endl;
	}
	if(in_this_pipe && (x_m > len || abs(y_m) > .5*p_width)){
		//cout << "out" << endl;
		in_this_pipe = false;
		agent->in_pipe = false;
	}
	return agent;
}

double Pipe::get_x_pipe(double x, double y){
	return x*cos(alpha) + y*sin(alpha) - cos(alpha)*x_pos_0 - sin(alpha)*y_pos_0;
}

double Pipe::get_y_pipe(double x, double y){
	return -x*sin(alpha) + y*cos(alpha) + sin(alpha)*x_pos_0 - cos(alpha)*y_pos_0;
}
