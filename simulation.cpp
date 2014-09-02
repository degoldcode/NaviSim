/*
 * simulation.cpp
 *
 *  Created on: 31.08.2014
 *      Author: meicron
 */

#include "simulation.h"

Simulation::Simulation(double param_start, double param_end, double dparam, int num_trials){
	distor.open("./data/distor.dat");
	gvlearn.open("./data/gvlearn.dat");
	//lvlearn.open("./data/lvlearn.dat");

	// Simulation parameters
	total_runs = num_trials;
	inbound_on = true;

	// Controller parameters
	num_neurons = 360;			//Number of array neurons
	num_motivs = 2;				//0=outbound,	1=inbound
	max_outbound_time = 500;
	max_inbound_time = 500;//600;
	motor_command = 0.0;

	// Environment parameters
	agents = 1;
	max_radius = 25.;
	goal_density = 0.02;//0.005;//0.0001;
	num_goals = int(goal_density * (M_PI * max_radius * max_radius));
	lm_density = 0.2;//0.02;//0.001;// 0.0001;
	num_landmarks = 0;//int(lm_density * (M_PI * m_radius * m_radius));

	// Evaluation
	PI_angular_error = 0.0;
	PI_linear_error = 0.0;
	num_goalhits = 0;
	num_homing = 0;

	controller = new NaviControl(num_neurons);											/////	+
	//environment = new Environment(agents/*, goals, landmarks, m_radius*/);
	environment = new Environment(agents, num_goals, num_landmarks, max_radius);
}

Simulation::~Simulation(){
	controller->save_matrices();

	delete controller;
	delete environment;

	gvlearn.close();
	distor.close();
	lvlearn.close();
}

void Simulation::run_sim(){
	for(int run = 0; run < total_runs; run++){
			//if(total_runs > 100)
				//controller->reset_matrices();
			double start_time = controller->t;
			double old_gvl = controller->gln->length;		//Old GV len
			while(controller->t < start_time + max_outbound_time && environment->sum_reward < 1.){																	//OUTBOUND RUN (SEARCHING)
				run_outbound();
			}
			double in_time = controller->t;
			while(inbound_on && environment->agent_list.at(0)->distance > 0.2 && controller->t < in_time + max_inbound_time){ 	//INBOUND RUN (PI HOMING)
				run_inbound();
			}
			//if(controller->gln->length - old_gvl < 0.000000001 && run > 20)	//convergence criterion
				//run = total_runs;
			if(environment->agent_list.at(0)->distance <= 0.2)
				num_homing++;
			num_goalhits += environment->get_hits();
			if(controller->expl_factor < 0.5)
				environment->agent_list.at(0)->short_write = false;
			gvlearn << run << " " << controller->t << " " << 1.0*num_goalhits/(run+1) << " " << 1.0*num_homing/(run+1) << " " << controller->expl_factor << endl;
			printf("Run = %3u, time@nest = %5u, hits/run = %2.3f, hits = %u, homing rate = %1.3f, dist_factor = %2.4f, expl_fact = %f, memory len = %1.3f\n", run+1, controller->t, (1.0*num_goalhits)/(1.0*(run+1)), num_goalhits, 1.0*num_homing/(run+1), controller->pin->length/environment->agent_list.at(0)->distance, controller->expl_factor, controller->pin->memory_length);
			environment->reset();
			controller->reset();
	}
}

void Simulation::run_outbound(){
	controller->set_outbound();
	environment->mode = 0;
	environment->update(motor_command);
	controller->get_pos(environment->getx(), environment->gety());
	motor_command = controller->update(environment->agent_list.at(0)->phi, environment->agent_list.at(0)->v, environment->reward, environment->lm_recogn);
	distor << environment->agent_list.at(0)->distance << " " << controller->pin->length << " " << controller->gln->length << endl;
	PI_angular_error = bound_angle(controller->PI_avg_angle - environment->agent_list.at(0)->theta);
	PI_linear_error = controller->pin->length - environment->agent_list.at(0)->distance;
	if(controller->t%500==0)
		printf("t = %4u\tPI_error_ang = %1.3f\tSumR = %2.3f\tValue = %2.3f\tPhi = %3.2f\tGV angle = %3.2f (%2.3f)\tLV angle = %3.2f (%2.3f)\n", controller->t, PI_angular_error, environment->sum_reward, controller->value, in_degr(environment->agent_list.at(0)->phi), in_degr(controller->GV_angle), controller->gln->length, in_degr(controller->LV_angle), controller->rln->length);
}

void Simulation::run_inbound(){
	controller->set_inbound();
	environment->mode = 1;
	motor_command = controller->update(environment->agent_list.at(0)->phi, environment->agent_list.at(0)->v, environment->reward, environment->lm_recogn);;
	environment->update(motor_command);
	if(environment->agent_list.at(0)->in_pipe)
		cout << "No. I'm in a pipe.\n";
	//if(controller->t%100==0)
	//printf("t = %4u\tFB_error = %1.3f\tPhi = %1.3f\n", controller->t, controller->feedback_error, environment->agent_list.at(0)->phi);
}

double Simulation::bound_angle(double phi){
	double rphi;
	rphi = phi;
	while(rphi > M_PI)
		rphi -= 2 * M_PI;
	while(rphi < - M_PI)
		rphi += 2 * M_PI;
	return rphi;
}

double Simulation::inv_angle(double angle){
	return bound_angle(angle - M_PI);
}

double Simulation::in_degr(double angle){
	return 180.*angle/M_PI;
}
