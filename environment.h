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
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class Environment {
public:
	Environment(double num_agents, double num_goals, double num_landmarks, double max_radius);
	~Environment();

	void update(double command);
	void add_goal(double x, double y);
	int get_hits();
	double get_distance(Goal* goal1, Goal* goal2);
	double get_distance(Landmark* landmark1, Goal* goal2);
	double get_distance(Landmark* landmark1, Landmark* landmark2);
	void reset();
	std::vector<Agent*> agent_list;
	std::vector<Goal*> goal_list;
	std::vector<Landmark*> landmark_list;

	double reward;
	vector<double> lm_recogn;
	bool flag;
	int count;

	ofstream stream_g;
	ofstream stream_lm;
};



#endif /* ENVIRONMENT_H_ */
