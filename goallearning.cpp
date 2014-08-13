/*
 * goallearning.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: degoldschmidt
 */

#include "goallearning.h"
using namespace std;


GoalLearning::GoalLearning(int neurons, int motiv_states, double nnoise){
	N = neurons;
	M = motiv_states;
	learn_rate = 1.0;
	reward = 0.0;
	max_angle = 0.0;

	pref_angle.zeros(N);
	act_pi_array.zeros(N);
	act_mu_array.zeros(M);
	act_gv_array.zeros(N);
	act_output.zeros(N);
	w_cos.zeros(N,N);
	w_mu_gv.zeros(N,M);
	//w_mu_gv.load("./save/weights.mat", raw_ascii);
	w_pi_gv.eye(N,N);

	mat One = eye<mat>(N/2,N/2);
	mat Zero = zeros<mat>(N/2,N/2);

	mat Upper = join_rows(Zero, One);
	mat Lower = join_rows(One, Zero);

	w_pi_gv = join_cols(Upper, Lower);

	for(int i = 0; i < N; i++)
		pref_angle(i) = (2.*M_PI*i)/N;
	for(int i = 0; i < N; i++)
		for(int j = 0; j < N; j++)
			w_cos(i,j) = 0.01*cos(pref_angle(i) - pref_angle(j));
	t = 0;
	sum_length = 0.0;
	length = 0.0;
	var = 0.0;
}

GoalLearning::~GoalLearning(){
	w_mu_gv.save("./save/weights.mat", raw_ascii);
}

vec GoalLearning::update(vec pi_input, double in_reward){
	act_pi_array = pi_input;
	reward = in_reward;
	update_activities();
	update_weights();
	var = mean(act_output);///mean(act_output);
	max_angle = get_max_angle(act_output);
	length =  7.686168886*get_max_value(act_output)/N;// + 0.00002*(t+1);
	t++;
	return act_output;
}

void GoalLearning::update_activities(){
	act_gv_array = w_mu_gv*act_mu_array;// + w_pi_gv*act_pi_array);
	act_output = act_gv_array; // 1.9119355*   //lin_rect(w_cos*act_gv_array) /*+ w_cos*act_output*/;//eye<mat>(act_gv_array.size(),act_gv_array.size())
}

void GoalLearning::update_weights(){
	dw_mu_gv = learn_rate * reward * (act_pi_array-act_gv_array) * act_mu_array.t();
	w_mu_gv += dw_mu_gv;
//	for(int i = 0; i < w_mu_gv.n_rows; i++)
//		for(int j = 0; j < w_mu_gv.n_cols; j++)
//			if(w_mu_gv(i,j) < 0.0)
//				w_mu_gv(i,j) = 0.0;
}

void GoalLearning::set_mu(double out, double in){
	vec input(2);
	input(0) = out;
	input(1) = in;
	act_mu_array = input;
}

double GoalLearning::get_max_angle(vec input){
	uword  index;
	double min_val = input.max(index);
	return pref_angle(index);
}

double GoalLearning::get_max_value(vec input){
	uword  index;
	double max_val = input.max(index);
	return max_val;
}

vec GoalLearning::lin_rect(vec input){
	vec copy = input;
	for(int i = 0; i < copy.n_rows; i++)
		if(copy(i) < 0.0)
			copy(i) = 0.0;
	return copy;
}
