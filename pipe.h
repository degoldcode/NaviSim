/*
 * pipe.h
 *
 *  Created on: 02.08.2014
 *      Author: meicron
 */

#ifndef PIPE_H_
#define PIPE_H_

#include "agent.h"

class Pipe {
public:
	Pipe(double x0, double x1, double y0, double y1, double width);
	~Pipe();

	Agent* set_agent_pipe(Agent* agent);
	double get_x_pipe(double x, double y);
	double get_y_pipe(double x, double y);

	double x_pos_0;
	double x_pos_1;
	double y_pos_0;
	double y_pos_1;
	double p_width;
	double alpha;
	double len;
	bool in_this_pipe;
};

#endif /* PIPE_H_ */
