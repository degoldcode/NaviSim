/*
 * pin.cpp
 *
 *  Created on: Jul 23, 2014
 *      Author: degoldschmidt
 */

#include "pin.h"


PIN::PIN(int p_neurons, double p_leak, double p_noise){
	N = p_neurons;
	leak_rate = p_leak;
	noise = p_noise;

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
	//---Layer 1 -> Head Direction Layer
	act_head_direction = cos(angle*ones<vec>(N) - pref_angle);
	//---Layer 2 -> Gater Layer
	act_gater = lin_rect(-eye<mat>(N,N)*act_head_direction + speed*ones<vec>(N));
	//---Layer 3 -> Memory Layer
	act_integrator = lin_rect(eye<mat>(N,N)*act_gater + (1.0-leak_rate)*eye<mat>(N,N)*act_integrator);
	//---Layer 4 -> Vector Decoding Layer
	act_output = lin_rect(w_cos * act_integrator);
	max_angle = get_max_angle(act_output);
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


PIN* pin;
Environment* environment;
Map* map;
const int num_neurons = 360;
const int total_time = 2000;
const int total_runs = 10;
const double factor = 0.5;
mat outputs(num_neurons, total_time*total_runs+1000*total_runs);
double PI_angle_error;

const int agents = 1;
const double m_radius = 100.;
const double g_density = 0.005;//0.0001;
const int goals = int(g_density * (M_PI * m_radius * m_radius));
const double lm_density = 0.0;//0.02;//0.001;// 0.0001;
const int landmarks = int(lm_density * (M_PI * m_radius * m_radius));
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

int main(){
	pin = new PIN(num_neurons, 0.0, 0.0);
	environment = new Environment(agents, goals, landmarks, m_radius);
	map = new Map(-20.);
	for(int run = 0; run < total_runs; run++){
		for(int t = 0; t < total_time; t++){
			//cout << run << " " << t <<endl;
			command = (2.*map->update_map() - 1.);
			outputs.col(t+total_time*run) = pin->update(environment->agent_list.at(0)->phi, 1.0);
			//		PI_angle_error = agent->theta - pin->max_angle;
			environment->update(command);
			//		if(t%100==0)
			//			cout << t << "\t" <<  PI_angle_error << "\t\t" << agent->theta << "\t\t" <<  bound_angle(pin->max_angle) << endl;
		}
		while(environment->agent_list.at(0)->distance > 0.5 && pin->t < total_time*(run+1) + 5000){
			outputs = join_rows(outputs, pin->update(environment->agent_list.at(0)->phi, 1.0));
			command = 0.95*(2.*map->update_map() - 1.) + 0.05*sin(bound_angle(pin->max_angle-environment->agent_list.at(0)->phi));
			environment->update(command);
		}
		printf("Run = %u, time@nest = %u\n", run+1, pin->t);
	}
	outputs.save("./data/out.mat", raw_ascii);
//	mat outputs_t = outputs.t();
//	outputs_t.save("./data/out_t.mat", raw_ascii);
	delete pin;
	delete environment;
	delete map;
}
