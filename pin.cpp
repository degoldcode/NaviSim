/*****************************************************************************
 *  pin.cpp                                                                  *
 *                                                                           *
 *  Created on:   Jul 23, 2014                                               *
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

#include "pin.h"


PIN::PIN(int num_neurons, double leak, double sens_noise, double neur_noise) : CircArray(num_neurons) {
	leak_rate = leak;
	snoise = sens_noise;
	nnoise = neur_noise;

	CircArray* in_array = new CircArray(N);
	ar.push_back(in_array);
	for(int i = HD+1; i <= HV; i++){
		CircArray* array = new CircArray(N,N);
		ar.push_back(array);
	}
}

PIN::~PIN(){
	for(int i = 0; i < ar.size(); i++)
		delete ar.at(i);
}

void PIN::reset(){
	for(int i = 0; i < ar.size(); i++)
		ar.at(i)->reset();
}

void PIN::update(double angle, double speed){
	//---Sensory Noise
	double noisy_angle = angle + 2.*M_PI*noise(snoise);
	double noisy_speed = speed + noise(snoise);

	//---Layer 1 -> Head Direction Layer
	ar.at(HD)->update_rate(cos(noisy_angle*ones<vec>(N) - preferred_angle)*(-0.5) + 0.5 + noise(nnoise));
	//---Layer 2 -> Gater Layer
	ar.at(G)->update_rate(lin_rect(-eye<mat>(N,N)*ar.at(HD)->rate() + noisy_speed*ones<vec>(N)) + noise(nnoise));
	//---Layer 3 -> Memory Layer
	ar.at(M)->update_rate(lin_rect(eye<mat>(N,N)*ar.at(G)->rate() + (1.0-leak_rate)*eye<mat>(N,N)*ar.at(M)->rate()) + noise(nnoise));
	//---Layer 4 -> Vector Decoding Layer
	ar.at(HV)->update_rate(lin_rect(w_cos * act_integrator) + gaussian_noise(nnoise));

	avg_angle = bound_PI_angle(get_avg_angle(act_output));
	max_angle = bound_PI_angle(get_max_angle(act_output));
	memory_length = sum(act_integrator)/N;
	length = 7.686168886*get_max_value(act_output)/N;// - 0.00408306293795454578;//*(t+1); // correcting term

	return act_output;
}

double PIN::bound_PI_angle(double phi){
	double rphi;
	int counter = 0;
	rphi = phi;
	while(rphi > M_PI){
		rphi -= 2 * M_PI;
		counter++;
		if(counter > 5){
			//cout << "Weird input angle: " << phi << ". Set to zero.\n";
			rphi= 0.0;
		}
	}
	while(rphi < - M_PI){
		rphi += 2 * M_PI;
		counter++;
		if(counter > 5){
			//cout << "Weird angle. Set to zero.\n";
			rphi= 0.0;
		}
	}
	return rphi;
}

double PIN::get_avg_angle(vec input){
	double sum_act = 0.0;
	double output;
	if(input(0) == 0.0){
		for(int i = 0; i < N; i++){
			if(act_output(i) > 0.0){
				output += pref_angle(i)*act_output(i);
				sum_act += act_output(i);
			}
		}
		output /= sum_act;
	}
	else{
		for(int i = 0; i < N; i++){
			if(act_output(i) > 0.0){
				output += bound_PI_angle(pref_angle(i))*act_output(i);
				sum_act += act_output(i);
			}
		}
		output /= sum_act;
	}
	return output;
}
