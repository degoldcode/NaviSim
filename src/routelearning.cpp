/*
 * routelearning.cpp
 *
 *  Created on: Aug 18, 2014
 *      Author: degoldschmidt
 */

#include "routelearning.h"

RouteLearning::RouteLearning(int num_neurons, int num_lmr_units, double nnoise, double* forage, bool opt_load) : CircArray(num_neurons, num_lmr_units) {
	local_vector.resize(num_lmr_units);
	foraging_state = forage;
	learn_rate = .5;
	reward = 0.0;
	neural_noise = nnoise;
	load_weights = opt_load;
	white_weights.zeros(N,K);
	if(load_weights)
		w().load("./save/routeweights.mat", raw_ascii);

	eligibility_lmr = zeros<vec>(num_lmr_units);
	reference_pin = new PIN(18, 0.0, 0.00, 0.0);

	printf("=== LV learning parameters ===\n");
	printf("Neurons: %u\n", N);
	printf("Landmark units: %u\n", K);
	printf("Learning rate: %g\n", learn_rate);
	printf("Uncorrelated noise: %g\n", nnoise);
	printf("Loading weights from file: %u\n", opt_load);
	printf("==============================\n\n");
}

RouteLearning::~RouteLearning(){
	delete reference_pin;
	input_conns.save("./save/routeweights.mat", raw_ascii);
}

double RouteLearning::el_lm(int index){
	return eligibility_lmr(index);
}

Vec RouteLearning::LV(int index){
	return local_vector.at(index);
}

vec RouteLearning::RefPI(){
	return reference_pin->get_output();
}

void RouteLearning::set_mu(double* state){
	foraging_state = state;
}

void RouteLearning::update(Angle angle, double speed, double in_reward, vec input_lmr) {

	//	if(reward > 0.0)
	//		printf("foraging state = %g\n", *foraging_state);
	vec input = input_lmr;//(1. - *foraging_state)*ones<vec>(K);
	double lowpass_elig = 0.999;
	eligibility_lmr = 0.1*input + lowpass_elig*eligibility_lmr;
	if(accu(input) > 0.5){
		reference_pin->reset();
	}

	//Reference PI
	double unoise = boost_unoise();
	//cout << unoise << endl;
	if(accu(eligibility_lmr) > 0.1)
		reference_pin->update(angle, speed);
	else
		reference_pin->update(Angle(2*M_PI*unoise), -0.00);
	update_rate(input_conns*input);
	update_weights();
	for(int index = 0; index < K; index++){
		set_avg(update_avg(input_conns.col(index)));
		set_len(update_len(input_conns.col(index)));
		set_max(update_max(input_conns.col(index)));
		local_vector.at(index).to(len(index)*avg(index).C(), len(index)*avg(index).S());
		if(input_conns.max() > 10000 || input_conns.min() < -1000)
			printf("Eta = %g\tR = %g\texp = %g\n", 1.-*foraging_state, reward, expl_rate);
	}
	//printf("(x,y) = (%g,%g)\tlen = %g\tavg = %g\n", GV(0).x, GV(0).y, len(), avg().deg());
}

void RouteLearning::update_weights(){
	vec ref_output = reference_pin->get_output();
	weight_change = learn_rate * reward /* eligibility_lmr*/ * (1. - *foraging_state) * (ref_output - input_conns) - /*0.0000004*/0.000001*input_conns;
	white_weights += weight_change;
	white_weights.elem( find(white_weights < 0.0) ).zeros();

	input_conns = white_weights+randu<vec>(N)*neural_noise;
}

