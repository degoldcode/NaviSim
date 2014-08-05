/*
 * navicontrol.h
 *
 *  Created on: 29.07.2014
 *      Author: meicron
 */

#ifndef NAVICONTROL_H_
#define NAVICONTROL_H_


#include <armadillo>
#include <cmath>
#include "pin.h"
#include "map.h"
#include "goallearning.h"
using namespace std;
using namespace arma;


class NaviControl {
public:
	NaviControl(int num_neurons);
	~NaviControl();

	double update(double angle, double speed, double reward);
	void update_matrices(vec PI, vec GL);
	double bound_angle(double phi);
	double inv_angle(double angle);
	void reset();
	void reset_matrices();
	void save_matrices();
	void set_inbound();
	void set_outbound();
	double rand(double mean, double stdev);

	PIN* pin;
	GoalLearning* gln;
	Map* map;

	mat outputs;
	mat out_array;
	mat gv_array;
	mat pi_array;
	mat mu_array;
	mat weight;
	mat dweight;
	double PI_angle_error;

	double map_output;
	double feedback_error;
	double goal_fb_error;
	double goal_factor;

	double PI_angle;
	double GV_angle;
	double CM_angle;		//CM = chaotic map

	int N;
	int t;

	ofstream stream;
};



#endif /* NAVICONTROL_H_ */
