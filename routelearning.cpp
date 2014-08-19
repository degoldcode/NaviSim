/*
 * routelearning.cpp
 *
 *  Created on: Aug 18, 2014
 *      Author: degoldschmidt
 */

#include "routelearning.h"

RouteLearning::RouteLearning(int neurons, int landmark_recog, double nnoise){
	N = neurons;
	M = landmark_recog;
	learn_rate = 1.0;
	reward = 0.0;
	lm_recogn = 0.0;
	lm_eligibility = 0.0;
	d_lm = 0.0;
	max_angle = 0.0;

	ref_pin = new PIN(N, 0.0000, 0.00, 0.00);

	pref_angle.zeros(N);
	act_pi_array.zeros(N);
	act_lmr_array.zeros(M);
	act_ref_array.zeros(N);
	act_lv_array.zeros(N);
	act_output.zeros(N);
	w_cos.zeros(N,N);
	w_lmr_lv.zeros(N,M);
	//w_lmr_gv.load("./save/routeweights.mat", raw_ascii);
	w_pi_lv.eye(N,N);

	mat One = eye<mat>(N/2,N/2);
	mat Zero = zeros<mat>(N/2,N/2);

	mat Upper = join_rows(Zero, One);
	mat Lower = join_rows(One, Zero);

	w_pi_lv = join_cols(Upper, Lower);

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

RouteLearning::~RouteLearning(){
	w_lmr_lv.save("./save/routeweights.mat", raw_ascii);
}

vec RouteLearning::update(vec pi_input, double in_reward, double in_lmr, double angle, double speed){
	act_pi_array = pi_input;
	reward = in_reward;
	d_lm = -lm_recogn;
	lm_recogn = in_lmr;
	d_lm += lm_recogn;
	lm_eligibility = 0.05*lm_recogn + 0.95*lm_eligibility;
	if(d_lm > 0.0)
		ref_pin->reset();
	act_ref_array = ref_pin->update(angle, speed);
	update_activities();
	update_weights();
	var = mean(act_output);///mean(act_output);
	max_angle = get_max_angle(act_output);
	length = 1.;//  7.686168886*get_max_value(act_output)/N;// + 0.00002*(t+1);
	t++;
	return act_output;
}

void RouteLearning::update_activities(){
	act_lmr_array(0) = lm_recogn;
	act_lv_array = w_lmr_lv*act_lmr_array;// + w_pi_gv*act_pi_array);
	act_output = act_lv_array; // 1.9119355*   //lin_rect(w_cos*act_gv_array) /*+ w_cos*act_output*/;//eye<mat>(act_gv_array.size(),act_gv_array.size())
}

void RouteLearning::update_weights(){
	dw_lmr_lv = learn_rate * reward * lm_eligibility * (act_ref_array-act_lv_array);
	w_lmr_lv += dw_lmr_lv;
//	for(int i = 0; i < w_mu_gv.n_rows; i++)
//		for(int j = 0; j < w_mu_gv.n_cols; j++)
//			if(w_mu_gv(i,j) < 0.0)
//				w_mu_gv(i,j) = 0.0;
}

double RouteLearning::get_max_angle(vec input){
	uword  index;
	double min_val = input.max(index);
	return pref_angle(index);
}

double RouteLearning::get_max_value(vec input){
	uword  index;
	double max_val = input.max(index);
	return max_val;
}

