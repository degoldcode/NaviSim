/*****************************************************************************
 *  goallearning.cpp                                                         *
 *                                                                           *
 *  Created on:   Jul 25, 2014                                               *
 *  Author:       Dennis Goldschmidt                                         *
 *  Email:        goldschmidtd@ini.phys.ethz.ch                              *
 *                                                                           *
 *                                                                           *
 *  Copyright (C) 2014 by Dennis Goldschmidt                                 *
 *                                                                           *
 *  This file is part of the program NaviSim                                 *
 *                                                                           *
 *  NaviSim is free software: you can redistribute it and/or modify          *
 *  it under the terms of the GNU General Public License as published by     *
 *  the Free Software Foundation, either version 3 of the License, or        *
 *  (at your option) any later version.                                      *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU General Public License for more details.                             *
 *                                                                           *
 *  You should have received a copy of the GNU General Public License        *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
 *                                                                           *
 ****************************************************************************/

#include "goallearning.h"
using namespace std;


GoalLearning::GoalLearning(int num_neurons, double nnoise, double* forage, bool opt_load) : CircArray(num_neurons,1) {
	type = 1;
	//threshold = 25.;
	global_vector.resize(1);
	foraging_state = forage;
	learn_rate = 5.;
	reward = 0.0;
	expl_rate =  0.0;
	neural_noise = nnoise;
	load_weights = opt_load;
	if(load_weights)
		w().load("./save/goalweights.mat", raw_ascii);

	printf("=== GV learning parameters ===\n");
	printf("Neurons: %u\n", num_neurons);
	printf("Goal types: %u\n", K);
	printf("Learning rate: %g\n", learn_rate);
	printf("Uncorrelated noise: %g\n", nnoise);
	printf("Loading weights from file: %u\n", opt_load);
	printf("==============================\n\n");
}

GoalLearning::~GoalLearning(){
	w().save("./save/goalweights.mat", raw_ascii);
}

Vec GoalLearning::GV(int index){
	return global_vector.at(index);
}

void GoalLearning::set_mu(double* state){
	foraging_state = state;
}

void GoalLearning::update(vec pi_input, double in_reward, double in_expl){
	reward = in_reward;
	expl_rate = in_expl;

//	if(reward > 0.0)
//		printf("foraging state = %g\n", *foraging_state);
	vec input = (1. - *foraging_state)*ones<vec>(K);
	update_rate(input_conns*input);
	update_weights(pi_input);
	set_avg(update_avg(input_conns.col(0)));
	set_len(update_len(input_conns.col(0)));
	set_max(update_max(input_conns.col(0)));

	if(input_conns.max() > 10000 || input_conns.min() < -1000)
		printf("Eta = %g\tR = %g\texp = %g\n", 1.-*foraging_state, reward, expl_rate);

	global_vector.at(0).to(len()*avg().C(), len()*avg().S());
	//printf("(x,y) = (%g,%g)\tlen = %g\tavg = %g\n", GV(0).x, GV(0).y, len(), avg().deg());
}

void GoalLearning::update_weights(vec pi_input){
	input_conns = input_conns+randu<vec>(N)*neural_noise;
	weight_change = learn_rate * reward /* expl_rate*/ * (1. - *foraging_state) * (pi_input-input_conns) - /*0.0000004*/0.000001*input_conns;
	input_conns += weight_change;
	input_conns.elem( find(input_conns < 0.0) ).zeros();
}

double GoalLearning::x(){
	return GV(0).x;
}

double GoalLearning::y(){
	return GV(0).y;
}
