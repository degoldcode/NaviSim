/*****************************************************************************
 *  pipe.cpp                                                                 *
 *                                                                           *
 *  Created on:   Aug 02, 2014                                               *
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
#include "pipe.h"
using namespace std;

Pipe::Pipe(double x0, double y0, double x1, double y1){
	x_pos_0 = x0;
	x_pos_1 = x1;
	y_pos_0 = y0;
	y_pos_1 = y1;
	double dx = x1 - x0;
	double dy = y1 - y0;
	alpha = atan2(dy, dx);
	len = sqrt(dx*dx + dy*dy);
}

Pipe::~Pipe(){

}

Agent* Pipe::set_agent_pipe(Agent* agent){
	double x_m = x_pipe(agent->x(), agent->y());
	double y_m = y_pipe(agent->x(), agent->y());
	if((x_m > -p_width && x_m < p_width && abs(y_m) < .5*p_width) || in_this_pipe){
		agent->set_dphi(alpha - agent->phi());
		if(!in_this_pipe){
			agent->set_x(x_pos_0);
			agent->set_y(y_pos_0);
		}
		in_this_pipe = true;	//puts agent in pipe
		agent->in_pipe = true;
	}
	if(in_this_pipe && (x_m > len || abs(y_m) > .5*p_width)){
		in_this_pipe = false;
		agent->in_pipe = false;					//gets agent out of pipe
	}
	return agent;
}

double Pipe::w(){
	return p_width;
}

double Pipe::x0(){
	return x_pos_0;
}

double Pipe::x1(){
	return x_pos_1;
}

double Pipe::x_pipe(double x, double y){
	return x*cos(alpha) + y*sin(alpha) - cos(alpha)*x_pos_0 - sin(alpha)*y_pos_0;
}

double Pipe::y0(){
	return y_pos_0;
}

double Pipe::y1(){
	return y_pos_1;
}

double Pipe::y_pipe(double x, double y){
	return -x*sin(alpha) + y*cos(alpha) + sin(alpha)*x_pos_0 - cos(alpha)*y_pos_0;
}
