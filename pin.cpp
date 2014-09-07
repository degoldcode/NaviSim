/*
 * pin.cpp
 *
 *  Created on: Jul 23, 2014
 *      Author: degoldschmidt
 */

#include "pin.h"


PIN::PIN(int p_neurons, double p_leak, double p_sens_noise, double p_neur_noise){
	N = p_neurons;
	leak_rate = p_leak;
	snoise = p_sens_noise;
	nnoise = p_neur_noise;

	act_head_direction.zeros(N);
	act_gater.zeros(N);
	act_integrator.zeros(N);
	act_output.zeros(N);
	pref_angle.zeros(N);
	w_cos.zeros(N,N);

	for(int i = 0; i < N; i++)
		pref_angle(i) = (2.*M_PI*i)/N;
	for(int i = 0; i < N; i++)
		for(int j = 0; j < N; j++)
			w_cos(i,j) = cos(pref_angle(i) - pref_angle(j));
	max_angle = 0.0;
	avg_angle = 0.0;
	length = 0.0;
	memory_length = 0.0;
	t = 0;
}

PIN::~PIN(){

}

void PIN::reset(){
	act_head_direction.zeros(N);
	act_gater.zeros(N);
	act_integrator.zeros(N);
	act_output.zeros(N);
	t = 0;
}

vec PIN::update(double angle, double speed){
	//---Sensory Noise
	double noisy_angle = angle + 2.*M_PI*gaussian_noise(snoise);
	double noisy_speed = speed + gaussian_noise(snoise);
	//printf("%g\t%g\n", snoise, noisy_angle - angle);
	//---Layer 1 -> Head Direction Layer
	act_head_direction = cos(noisy_angle*ones<vec>(N) - pref_angle)*(-0.5) + 0.5 + gaussian_noise(nnoise);
	//---Layer 2 -> Gater Layer
	act_gater = lin_rect(-eye<mat>(N,N)*act_head_direction + noisy_speed*ones<vec>(N)) + gaussian_noise(nnoise);
	//---Layer 3 -> Memory Layer
	act_integrator = lin_rect(eye<mat>(N,N)*act_gater + (1.0-leak_rate)*eye<mat>(N,N)*act_integrator) + gaussian_noise(nnoise);
	//---Layer 4 -> Vector Decoding Layer
	act_output = lin_rect(w_cos * act_integrator) + gaussian_noise(nnoise);

	avg_angle = bound_PI_angle(get_avg_angle(act_output));
	max_angle = bound_PI_angle(get_max_angle(act_output));
	memory_length = sum(act_integrator)/N;
	length = 7.686168886*get_max_value(act_output)/N;// - 0.00408306293795454578;//*(t+1); // correcting term
	//if(abs(180.*max_angle/M_PI - 180.*avg_angle/M_PI) > 1.)
		//printf("max = %3.3f\tavg = %3.3f\n", 180.*max_angle/M_PI, 180.*avg_angle/M_PI);
	//---Next timestep
	t++;
	return act_output;
}

vec PIN::lin_rect(vec input){
	vec copy = input;
	for(int i = 0; i < copy.n_rows; i++)
		if(copy(i) < 0.0)
			copy(i) = 0.0;
	return copy;
}

double PIN::get_max_angle(vec input){
	uword  index;
	double max_val = input.max(index);
	return pref_angle(index);
}

double PIN::get_max_value(vec input){
	uword  index;
	double max_val = input.max(index);
	return max_val;
}

double PIN::gaussian_noise(double width){
	if(width > 0.0){
		static random_device e{};
		static normal_distribution<double> d(0., sqrt(width));
		return d(e);
	}
	else
		return 0.;
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

double PIN::maxeigenvalue(mat & A){
	cx_vec eigval;
	cx_mat eigvec;
	// calculate eigenvalues for a non-hermitian matrix
	eig_gen(eigval, eigvec, A);
	// output of max() is cx_mat (complex) and must first be converted to mat (double)
	mat maxVal = max(abs(eigval));
	return maxVal(0);
}

//double PIN::get_activity_width(vec input){
//
//}

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
