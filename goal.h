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
	Goal(double x, double y, int color = 0);
	~Goal();

	double a();
	double x();
	double y();
	double d();
	double th();
	void swap();
	int type();
	double r(double x, double y, int mode);
	double rand(double min, double max);
	int hit;
	int total_hits;

private:
	double x_position;
	double y_position;
	double distance_to_origin;
	double angle_to_x_axis;

	double amount;
	double amount_rate;
	double factor;
	int goal_type;
};



#endif /* GOAL_H_ */
