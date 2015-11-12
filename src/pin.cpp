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

PIN::PIN(int num_neurons, double leak, double sens_noise, double neur_noise, bool in_silent) : CircArray(num_neurons) {
	t_step = 0;
	SILENT = in_silent;
	VERBOSE = false;
	if(!SILENT){
		printf("=== PI parameters ============\n");
		printf("Neurons: %u\n", N);
		leak_rate = leak;
		printf("Leak: %g\n", leak_rate);
		snoise = sens_noise;
		printf("Sensory noise: %g\n", snoise);
		nnoise = neur_noise;
		printf("Uncorrelated noise: %g\n", nnoise);
		printf("==============================\n\n");
	}
//	VERBOSE = true;

	CircArray* in_array = new CircArray(N);
	ar.push_back(in_array);
	for(int i = HD+1; i <= PI; i++){
		CircArray* array = new CircArray(N,N);
		ar.push_back(array);
	}
	w_cos = ar.at(PI)->cos_kernel();
}

PIN::~PIN(){
	for(int i = 0; i < ar.size(); i++)
		delete ar.at(i);
}

CircArray* PIN::array(int i){
	return ar.at(i);
}

vec PIN::get_output(){
	return ar.at(PI)->rate();
}

Vec PIN::HV(){
	return home_vector;
}
Vec PIN::HVm(){
	return home_vector_max;
}

void PIN::reset(){
	for(int i = 0; i < ar.size(); i++)
		ar.at(i)->reset();
}

void PIN::update(Angle angle, double speed){
	t_step++;
	//---Sensory Noise
	Angle noisy_angle = angle + Angle(2.*M_PI*snoise*boost_noise(1.));
	double noisy_speed = speed + 0.1*snoise*boost_noise(1.);
	if(noisy_speed < 0.0)
		noisy_speed = 0.0;

	//---Layer 1 -> Head Direction Layer
	vec input = cos(noisy_angle.rad()*ones<vec>(N) - preferred_angle)*(-0.5) + 0.5*ones<vec>(N) + vnoise(N,nnoise);
	// Multiplicative modulation:
	//vec input = cos(noisy_angle.rad()*ones<vec>(N) - preferred_angle) + vnoise(N,nnoise);

	ar.at(HD)->update_rate(input);

	//---Layer 2 -> Gater Layer
	ar.at(G)->update_rate(lin_rect(-eye<mat>(N,N)*ar.at(HD)->rate()+(noisy_speed)*ones<vec>(N)) /*+ vnoise(N,nnoise)*/);
	// Multiplicative modulation:
	//vec gater_input = eye<mat>(N,N)*ar.at(HD)->rate();
	//ar.at(G)->update_rate( lin_rect(gater_input*noisy_speed) );

	//---Layer 3 -> Memory Layer
	ar.at(M)->update_rate(lin_rect(eye<mat>(N,N)*ar.at(G)->rate() + (1.0-leak_rate)*eye<mat>(N,N)*ar.at(M)->rate()) /*+ vnoise(N,nnoise)*/);
	//---Layer 4 -> Vector Decoding Layer
	ar.at(PI)->update_rate(lin_rect(w_cos * ar.at(M)->rate()) /*+ vnoise(N,nnoise)*/);

	//Output parameters
	vec out = ar.at(PI)->rate();

	//*** Update vector representation ***//
	/// home vector TODO
	Angle hv_angle = vector_avg(out);
	Angle hv_angle_new = pva_angle(out);
	double hv_len = pva_len(out);
	//local_vector.to(lv_len*lv_angle.C(), lv_len*lv_angle.S());


	//printf("%g\n", accu(out));
	update_piavg(out);
	update_pilen(out);
	set_max(update_max(out), 0);
	home_vector.to(len()*avg().C(), len()*avg().S());
	home_vector_max.to(len()*max().C(), len()*max().S());
}

double PIN::x(){
	return home_vector.x;
}

double PIN::y(){
	return home_vector.y;
}
