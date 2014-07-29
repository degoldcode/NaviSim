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
	t = 0;
}

PIN::~PIN(){

}

vec PIN::update(double angle, double speed){
	//---Sensory Noise
	double noisy_angle = angle + 2.*M_PI*gaussian_noise(snoise);
	double noisy_speed = speed + gaussian_noise(snoise);
	//---Layer 1 -> Head Direction Layer
	act_head_direction = cos(noisy_angle*ones<vec>(N) - pref_angle)*(-0.5) + 0.5 + gaussian_noise(nnoise);
	//---Layer 2 -> Gater Layer
	act_gater = lin_rect(-eye<mat>(N,N)*act_head_direction + noisy_speed*ones<vec>(N)) + gaussian_noise(nnoise);
	//---Layer 3 -> Memory Layer
	act_integrator = lin_rect(eye<mat>(N,N)*act_gater + (1.0-leak_rate)*eye<mat>(N,N)*act_integrator) + gaussian_noise(nnoise);
	//---Layer 4 -> Vector Decoding Layer
	act_output = lin_rect(w_cos * act_integrator) + gaussian_noise(nnoise);
	max_angle = bound_PI_angle(get_max_angle(act_output));
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
	double min_val = input.max(index);
	return pref_angle(index);
}

double PIN::gaussian_noise(double width){
	if(width > 0.0){
		static random_device e{};
		static normal_distribution<double> d(0.0, width);
		return d(e);
	}
	else
		return 0.;
}

double PIN::bound_PI_angle(double phi){
	double rphi;
	rphi = phi;
	while(rphi > M_PI)
		rphi -= 2 * M_PI;
	while(rphi < - M_PI)
		rphi += 2 * M_PI;
	return rphi;
}

//**********************************************************

PIN* pin;
GoalLearning* gln;
Environment* environment;
Map* map;
const int num_neurons = 360;			//Number of array neurons
const int num_motivs = 2;				//0=outbound,	1=inbound
const int total_time = 1000;
const int max_extra_time = 2000;
const int total_runs = 5;
const double factor = 0.5;
mat outputs;
mat gv_array;
mat pi_array;
mat mu_array;
mat weight;
mat dweight;
double PI_angle_error;

const int agents = 1;
const double m_radius = 100.;
const double g_density = 0.01;//0.005;//0.0001;
const int goals = int(g_density * (M_PI * m_radius * m_radius));
const double lm_density = 0.0;//0.02;//0.001;// 0.0001;
const int landmarks = int(lm_density * (M_PI * m_radius * m_radius));
double feedback_error;
double goal_fb_error;
double goal_factor;
double command;

double bound_angle(double phi){
	double rphi;
	rphi = phi;
	while(rphi > M_PI)
		rphi -= 2 * M_PI;
	while(rphi < - M_PI)
		rphi += 2 * M_PI;
	return rphi;
}

double inv_angle(double angle){
	return bound_angle(angle - M_PI);
}

int main(){
	pin = new PIN(num_neurons, 0.0, 0.0, 0.0);
	gln = new GoalLearning(num_neurons, num_motivs, 0.0);
	environment = new Environment(agents, goals, landmarks, m_radius);
	environment->add_goal(20.,0.);
	map = new Map(-20.);
	for(int run = 0; run < total_runs; run++){
		environment->reset();
		if(total_runs > 20){
			outputs.reset();
			pi_array.reset();
			mu_array.reset();
			gv_array.reset();
			weight.reset();
			dweight.reset();
		}
		for(int t = 0; t < total_time; t++){																	//OUTBOUND RUN (SEARCHING)
			//cout << run << " " << t <<endl;
			gln->set_mu(1.0, 0.0);
			outputs = join_rows(outputs, pin->update(environment->agent_list.at(0)->phi, environment->agent_list.at(0)->v));
			gv_array = join_rows(gv_array, gln->update(outputs.col(outputs.n_cols-1), environment->reward));
			pi_array = join_rows(pi_array, gln->act_pi_array);
			mu_array = join_rows(mu_array, gln->act_mu_array);
			weight = join_rows(weight, gln->w_mu_gv.col(0));
			dweight = join_rows(dweight, gln->dw_mu_gv.col(0));
			if(environment->reward > 0.0)
				cout << pin->t << " " << environment->reward << endl;
			PI_angle_error = bound_angle(pin->max_angle - environment->agent_list.at(0)->theta);
			environment->update(command);
			feedback_error = bound_angle(gln->max_angle-environment->agent_list.at(0)->phi);
			goal_factor = (tanh(max(max(gln->w_mu_gv))/num_neurons));
			command = /*(1.0-goal_factor)*(2.*map->update_map() - 1.) +*/ goal_factor*sin(feedback_error);
			if(pin->t%1000==0)
				printf("t = %4u\tPI_error = %1.3f\tHV = %1.3f\tTheta = %1.3f\tinv HV = %1.3f\tPhi = %1.3f\tGV angle = %1.3f (%1.3f)\n", pin->t, PI_angle_error, environment->agent_list.at(0)->theta, pin->max_angle, bound_angle(pin->max_angle-M_PI), environment->agent_list.at(0)->phi, gln->max_angle, goal_factor);
		}
		double in_time = pin->t;
		while(environment->agent_list.at(0)->distance > 0.5 && pin->t < in_time + max_extra_time){ 	//INBOUND RUN (PI HOMING)
			gln->set_mu(0.0, 1.0);
			outputs = join_rows(outputs, pin->update(environment->agent_list.at(0)->phi, environment->agent_list.at(0)->v));
			gv_array = join_rows(gv_array, gln->update(outputs.col(outputs.n_cols-1), environment->reward));
			pi_array = join_rows(pi_array, gln->act_pi_array);
			mu_array = join_rows(mu_array, gln->act_mu_array);
			weight = join_rows(weight, gln->w_mu_gv.col(0));
			dweight = join_rows(dweight, gln->dw_mu_gv.col(0));
			feedback_error = 0.1*bound_angle(inv_angle(pin->max_angle)-environment->agent_list.at(0)->phi);
			if(pin->t%1000==0)
				printf("t = %4u\tFB_error = %1.3f\tHV = %1.3f\tTheta = %1.3f\tinv HV = %1.3f\tPhi = %1.3f\n", pin->t, feedback_error, environment->agent_list.at(0)->theta, pin->max_angle, bound_angle(pin->max_angle-M_PI), environment->agent_list.at(0)->phi);
			command = /*0.95*/0.75*(2.*map->update_map() - 1.) + 0.25/*0.05*/*sin(feedback_error);
			environment->update(command);
		}
		printf("Run = %u, time@nest = %u\n", run+1, pin->t);
	}
	outputs.save("./data/out.mat", raw_ascii);
	pi_array.save("./data/pi.mat", raw_ascii);
	mu_array.save("./data/mu.mat", raw_ascii);
	gv_array.save("./data/gv.mat", raw_ascii);
	weight.save("./data/w.mat", raw_ascii);
	dweight.save("./data/dw.mat", raw_ascii);
//	mat outputs_t = outputs.t();
//	outputs_t.save("./data/out_t.mat", raw_ascii);
	delete pin;
	delete gln;
	delete environment;
	delete map;
}
