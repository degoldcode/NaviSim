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


GoalLearning::GoalLearning(int num_neurons, double nnoise, bool opt_load) : CircArray(num_neurons,1) {
	type = 1;
	foraging_state = 1.0;
	learn_rate = 5.;
	reward = 0.0;
	expl_rate =  0.0;
	load_weights = opt_load;
	if(load_weights)
		w().load("./save/goalweights.mat", raw_ascii);
}

GoalLearning::~GoalLearning(){
	w().save("./save/goalweights.mat", raw_ascii);
}

Vec GoalLearning::GV(int index){
	return global_vector.at(index);
}

void GoalLearning::set_mu(double state){
	foraging_state = state;
}

void GoalLearning::update(vec pi_input, double in_reward, double in_expl){
	reward = in_reward;
	expl_rate = in_expl;

	vec input = foraging_state*ones<vec>(K);
	update_rate(input_conns*input);
	update_weights(pi_input);
	GV(0).to(len()*avg().C(), len()*avg().S());	//TODO
}

void GoalLearning::update_weights(vec pi_input){
	weight_change = learn_rate * reward * expl_rate * foraging_state * (pi_input-input_conns);
	input_conns += weight_change;
}

double GoalLearning::x(){
	return GV(0).x;
}

double GoalLearning::y(){
	return GV(0).y;
}
