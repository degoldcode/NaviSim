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
#include "routelearning.h"
using namespace std;
using namespace arma;


class NaviControl {
public:
	NaviControl(int num_neurons);
	~NaviControl();

	void get_pos(double x, double y);
	double update(double angle, double speed, double reward, double lm_recogn);
	void update_matrices(vec PI, vec GL, vec RL);
	double bound_angle(double phi);
	double inv_angle(double angle);
	double in_degr(double angle);
	void reset();
	void reset_matrices();
	void save_matrices();
	void set_inbound();
	void set_outbound();
	double rand(double mean, double stdev);

	PIN* pin;
	GoalLearning* gln;
	RouteLearning* rln;
	Map* map;

	mat outputs;
	mat out_array;
	mat gv_array;
	mat pi_array;
	mat lv_array;
	mat ref_array;
	mat mu_array;
	mat gv_weight;
	mat lv_weight;
	mat dweight;
	double PI_angle_error;

	double map_output;
	double feedback_error;
	double goal_fb_error;
	double goal_factor;

	double PI_max_angle;
	double PI_avg_angle;
	double PI_x;
	double PI_y;
	double GV_angle;
	double GV_x;
	double GV_y;
	double cGV_angle;
	double cLV_angle;
	double LV_angle;
	double LV_x;
	double LV_y;
	double CM_angle;		//CM = chaotic map
	double lm_lowpass;
	double accu_reward;
	double disc_factor;
	double expl_factor;
	double rx;
	double ry;

	int N;
	int t;
	int inv_sampling_rate;		//for activations stored in matrix ([s])

	bool start_fixed;

	ofstream stream;
	ofstream r_stream;
	ofstream lm_stream;
};



#endif /* NAVICONTROL_H_ */
