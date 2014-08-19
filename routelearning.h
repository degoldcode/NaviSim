/*
 * routelearning.h
 *
 *  Created on: Aug 18, 2014
 *      Author: degoldschmidt
 */

#ifndef ROUTELEARNING_H_
#define ROUTELEARNING_H_

#include <armadillo>
#include "pin.h"
using namespace arma;

class RouteLearning {
public:
	RouteLearning(int neurons, int landmark_recog, double nnoise);
	~RouteLearning();

	vec update(vec pi_input, double in_reward, double in_lmr, double angle, double speed);
	void update_activities();
	void update_weights();
	void set_mu(double out, double in);
	double get_max_angle(vec input);
	double get_max_value(vec input);
	vec lin_rect(vec input);

	PIN * ref_pin;

	vec pref_angle;
	vec act_pi_array;
	vec act_lmr_array;
	vec act_ref_array;
	vec act_lv_array;
	vec act_output;
	mat w_cos;

	mat w_lmr_lv;
	mat dw_lmr_lv;
	mat w_pi_lv;

	int N;
	int M;
	double learn_rate;
	double reward;
	double lm_recogn;
	double d_lm;
	double lm_eligibility;
	double max_angle;
	double sum_length;
	double length;
	double var;
	int t;
};



#endif /* ROUTELEARNING_H_ */
