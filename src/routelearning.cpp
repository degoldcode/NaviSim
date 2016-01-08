/*
 * routelearning.cpp
 *
 *  Created on: Aug 18, 2014
 *      Author: degoldschmidt
 */

#include "routelearning.h"

RouteLearning::RouteLearning(int num_neurons, int num_lmr_units, double nnoise, double* forage, bool opt_load, bool in_silent) : CircArray(num_neurons, num_lmr_units) {
	t_step = 0;
	no_learning = false;
	VERBOSE = false;
	SILENT = in_silent;
	stored_local_vector.resize(num_lmr_units);
	new_vector_avg.resize(num_lmr_units);
	foraging_state = forage;
	learn_rate = 1.;
	reward = 0.0;
	neural_noise = nnoise;
	load_weights = opt_load;
	weight_change.zeros(N,K);
	white_weights.zeros(N,K);
	//printf("%u X %u\n", white_weights.n_rows, white_weights.n_cols);
	if(load_weights){
		white_weights.load("./save/routeweights.mat", raw_ascii);
		input_conns.load("./save/routeweights.mat", raw_ascii);
		printf("Load weights: %f\n", accu(input_conns));
	}

	eligibility_lmr = zeros<vec>(num_lmr_units);
	eligibility_long = zeros<vec>(num_lmr_units);
	raw_lmr = zeros<vec>(num_lmr_units);
	d_raw_lmr = zeros<vec>(num_lmr_units);
	clip_lmr = zeros<vec>(num_lmr_units);
	value = zeros<vec>(num_lmr_units);
	reference_pin = new PIN(N, 0.0, 0.00, 0.0);

	if(!SILENT){
		printf("=== LV learning parameters ===\n");
		printf("Neurons: %u\n", N);
		printf("Landmark units: %u\n", K);
		printf("Learning rate: %g\n", learn_rate);
		printf("Uncorrelated noise: %g\n", nnoise);
		printf("Loading weights from file: %u\n", opt_load);
		printf("==============================\n\n");
	}
}

RouteLearning::~RouteLearning(){
	delete reference_pin;
	input_conns.save("./save/routeweights.mat", raw_ascii);
	printf("Save weights: %f\n", accu(w()));
}

double RouteLearning::cl_state_lm(int index){
	return clip_lmr(index);
}


mat RouteLearning::dW(){
	return weight_change;
}

double RouteLearning::el_lm(int index){
	return eligibility_lmr(index);
}

Vec RouteLearning::LV(){
	for(unsigned int index = 0; index < K; index++){
		if(stored_local_vector.at(index).lock() && (el_lm(index) > 0.0 || state_lm(index) > 0.5))
			return stored_local_vector.at(index);
	}
	return local_vector;
}

Vec RouteLearning::LV(int index){
	return stored_local_vector.at(index);
}

void RouteLearning::LV(int index, Vec vector, bool locked){
	stored_local_vector.at(index).to(vector);
	stored_local_vector.at(index).lock(locked);
}

double RouteLearning::R(){
	return reward;
}

double RouteLearning::R(int index){
	return value(index)*raw_lmr(index);
}

vec RouteLearning::RefPI(){
	return reference_pin->get_output();
}

Vec RouteLearning::RV(){
	return reference_pin->HV();
}

void RouteLearning::reset_el_lm(){
	t_step = 0;
	reference_pin->reset();
	eligibility_lmr = zeros<vec>(K);
	eligibility_long = zeros<vec>(K);
}

void RouteLearning::set_mu(double* state){
	foraging_state = state;
}

double RouteLearning::dstate_lm(int index){
	return d_raw_lmr(index);
}

double RouteLearning::state_lm(int index){
	return raw_lmr(index);
}

void RouteLearning::update(Angle angle, double speed, double in_reward, vec input_lmr) {
	t_step++;
	//	if(reward > 0.0)
	//		printf("foraging state = %g\n", *foraging_state);
	d_raw_lmr = -raw_lmr;
	raw_lmr = input_lmr;//(1. - *foraging_state)*ones<vec>(K);
	if(raw_lmr.n_elem < eligibility_lmr.n_elem){
		raw_lmr.resize(eligibility_lmr.n_elem);
	}
	d_raw_lmr += raw_lmr;
	d_raw_lmr *= -1;

	//printf("LMR_dim = %u\n", raw_lmr.n_elem);
	clip_lmr = d_raw_lmr;
	clip_lmr.elem( find(clip_lmr < 0.0) ).zeros();
	clip_lmr.elem( find(clip_lmr > 0.0) ).ones();
	double lowpass_elig = 0.995;	//0.995
	eligibility_long = 1.0*clip_lmr + lowpass_elig*eligibility_long;
	eligibility_long.elem( find(raw_lmr > 0.0) ).zeros();
	eligibility_long.elem( find(eligibility_lmr > 1.0) ).ones();
	eligibility_lmr = eligibility_long;

	/// only maximum active eligibility trace is chosen
	double emax = arma::max(eligibility_lmr);
	for(int i=0; i<eligibility_lmr.n_elem; i++){
		if(eligibility_lmr(i) < emax)
			eligibility_lmr(i) = 0.0;
	}

	if(accu(clip_lmr) > 0.5/*accu(raw_lmr) > 0.5 || accu(eligibility_lmr) < 0.1*/){
		reference_pin->reset();
		if(VERBOSE)
			printf("t = %u\n", t_step);
	}
	value += (in_reward - value_decay)*eligibility_long - global_decay*value;
	value.elem( find(value < 0.0) ).zeros();
	vec value_new = 1. - exp(-value);

	double lv_value_reward = as_scalar(value_new.t()*raw_lmr);/// value_k * sigma_k
	reward = in_reward + lv_value_reward;


	//Reference PI
	double unoise = boost_unoise();
	if(VERBOSE && t_step%10==0)
		printf("angle, speed = %g, %g\n", angle.deg(), speed);
	reference_pin->update(angle, speed);
	if(VERBOSE && t_step%10==0)
		printf("t = %u, RV = (%g,%g)\t(%f, %f)\n", t_step, reference_pin->HV().x, reference_pin->HV().y, reference_pin->HV().ang().deg(), reference_pin->HV().len());
	update_rate(input_conns*sign(eligibility_lmr));
	double deltaW = -accu(input_conns);
	update_weights();
	deltaW += accu(input_conns);
	if(VERBOSE && abs(deltaW) > 0.)
		printf("t = %u\t postw weights: %f\n", t_step, accu(input_conns));
	vec output = rate();

	//*** Update vector representations ***//
	/// active local vector
	Angle lv_angle = vector_avg(output);
	Angle lv_angle_new = pva_angle(output);
	double lv_len = pva_len(output);
	local_vector.to(lv_len*lv_angle.C(), lv_len*lv_angle.S());
	if(VERBOSE && t_step%100==0)
		printf("LV =(%g,%g)\n", local_vector.x, local_vector.y);

	/// stored local vector
	for(int index = 0; index < K; index++){
//		set_avg(update_avg(input_conns.col(index)));
//		set_len(update_len(input_conns.col(index)));
//		set_max(update_max(input_conns.col(index)));
		Angle lv_angle = vector_avg(input_conns.col(index));
		double lv_len = pva_len(input_conns.col(index));
		stored_local_vector.at(index).to(lv_len*lv_angle.C(), lv_len*lv_angle.S());
		if(VERBOSE && t_step%100==0)
			printf("LV%u =(%g,%g)\n", index, stored_local_vector.at(index).x, stored_local_vector.at(index).y);
		//if(input_conns.max() > 10000 || input_conns.min() < -1000)
			//printf("RV%u Eta = %g\tR = %g\n", index, 1.-*foraging_state, reward);
	}
	if(VERBOSE && t_step%100==0)
		printf("\n");
}

void RouteLearning::update_weights(){
	vec ref_output = reference_pin->get_output();
	for(int i = 0; i < K; i++){
		vec diff_act = ref_output - input_conns.col(i);
		weight_change.col(i) = learn_rate * reward * eligibility_lmr(i) * (1. - *foraging_state) * diff_act;// - /*0.0000004*/0.000001*input_conns;
		if(VERBOSE && t_step%1==0 && abs(accu(weight_change.col(i))) > 0.1){
			printf("$t= %u Learning for LV %u: el_lm = %f, R = %f, sum(W) = %f, sum(dW) = %f\n", t_step, i, eligibility_lmr(i), reward, accu(input_conns.col(i)), accu(weight_change.col(i)));
			printf("RV =(%g,%g)\n", reference_pin->HV().x, reference_pin->HV().y);
		}
	}
	white_weights += weight_change;
	white_weights.elem( find(white_weights < 0.0) ).zeros();
	input_conns = white_weights+randu<mat>(N,K)*neural_noise;
}

Angle RouteLearning::vec_avg(int _index){
	return new_vector_avg.at(_index);
}

double RouteLearning::eligibility_value(int _index){
	return eligibility_lmr(_index)*value(_index);
}

double RouteLearning::lv_value(int _index){
	return value(_index);
}

void RouteLearning::lv_value(int _index, double _value){
	value(_index) = _value;
}

