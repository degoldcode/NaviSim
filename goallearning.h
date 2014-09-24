/*
 * goallearning.h
 *
 *  Created on: Jul 25, 2014
 *      Author: degoldschmidt
 */

#ifndef GOALLEARNING_H_
#define GOALLEARNING_H_

#include <armadillo>
using namespace arma;

class GoalLearning {
public:
	GoalLearning(int neurons, int motiv_states, double nnoise);
	~GoalLearning();

	vec update(vec pi_input, double in_reward);
	void update_activities();
	void update_weights();
	void set_mu(double state, int index);
	double get_max_angle(vec input);
	double get_max_value(vec input);
	vec lin_rect(vec input);

	vec pref_angle;
	vec act_pi_array;
	vec act_mu_array;
	vec act_gv_array;
	vec act_output;
	mat w_cos;

	mat w_mu_gv;
	mat dw_mu_gv;
	mat w_pi_gv;

	int N;
	int M;
	double learn_rate;
	double reward;
	double max_angle;
	double sum_length;
	double length;
	double var;
	int t;
};



#endif /* GOALLEARNING_H_ */
