/*
 * goal.h
 *
 *  Created on: Jul 17, 2014
 *      Author: degoldschmidt
 */

#ifndef GOAL_H_
#define GOAL_H_

using namespace std;

class Goal {
public:
	Goal(double max_radius);
	Goal(double x, double y);
	~Goal();

	double x_position;
	double y_position;
	double distance_to_origin;
	double angle_to_x_axis;

	double get_reward(double x, double y, int mode);
	double rand(double min, double max);
	int hit;
	int total_hits;

	double amount;
};



#endif /* GOAL_H_ */
