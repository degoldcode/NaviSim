/*
 * environment.h
 *
 *  Created on: Jul 17, 2014
 *      Author: degoldschmidt
 */

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "agent.h"
#include "goal.h"
#include "landmark.h"
#include "pipe.h"
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class Environment {
public:
	Environment(double num_agents); //Empty environment
	Environment(double num_agents, double num_goals, double num_landmarks, double max_radius);
	~Environment();

	void update(double command);
	void add_goal(double x, double y);
	void add_pipe(double x0, double x1, double y0, double y1, double width);
	int get_hits();
	double get_distance(Goal* goal1, Goal* goal2);
	double get_distance(Landmark* landmark1, Goal* goal2);
	double get_distance(Landmark* landmark1, Landmark* landmark2);
	void reset();
	vector<Agent*> agent_list;
	vector<Goal*> goal_list;
	vector<Landmark*> landmark_list;
	vector<Pipe*> pipe_list;

	double reward;
	double sum_reward;
	vector<double> lm_recogn;
	bool flag;
	int count;

	ofstream stream_g;
	ofstream stream_lm;
	ofstream stream_p;
};

#endif /* ENVIRONMENT_H_ */
