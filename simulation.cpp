/*
 * simulation.cpp
 *
 *  Created on: 31.08.2014
 *      Author: meicron
 */

#include "simulation.h"

Simulation::Simulation(string in_param_type, int in_num_trials){
	file_name.str(string());
	file_name << "./data/PIerror_" << in_param_type << ".dat";

	distor.open("./data/distor.dat");
	gvlearn.open("./data/gvlearn.dat");
	PI_results.open(file_name.str().c_str());

	file_name.str(string());
	file_name << "./data/PIerroreach_" << in_param_type << ".dat";
	PI_resultseach.open(file_name.str().c_str());
	//lvlearn.open("./data/lvlearn.dat");

	// Simulation parameters
	total_runs = in_num_trials;
	inbound_on = false;
	num_params = 0;
	file_ext = in_param_type;

	//*** DEFAULT VALUES **//

	// Controller parameters
	num_neurons = 360;			//Number of array neurons
	sens_noise = 0.00;
	pi_leakage = 0.00;
	num_motivs = 2;				//0=outbound,	1=inbound
	max_outbound_time = 100;
	max_inbound_time = 10;//600;
	motor_command = 0.0;

	// Environment parameters
	agents = 1;
	max_radius = 25.;
	goal_density = 0.02;//0.005;//0.0001;
	num_goals = int(goal_density * (M_PI * max_radius * max_radius));
	lm_density = 0.2;//0.02;//0.001;// 0.0001;
	num_landmarks = 0;//int(lm_density * (M_PI * m_radius * m_radius));

	// Evaluation
	num_goalhits = 0;
	num_homing = 0;

	controller = new NaviControl(num_neurons, sens_noise, pi_leakage);											/////	+
	//environment = new Environment(agents/*, goals, landmarks, m_radius*/);
	environment = new Environment(agents, num_goals, num_landmarks, max_radius);
}

Simulation::~Simulation(){
	controller->save_matrices();

	delete controller;
	delete environment;

	PI_results.close();
	PI_resultseach.close();
	gvlearn.close();
	distor.close();
	lvlearn.close();
}

void Simulation::run_sim(double param){
	delete controller;
	delete environment;

	controller = new NaviControl(num_neurons, sens_noise, pi_leakage);
	printf("set param -> %f\n", controller->pin->snoise);
	environment = new Environment(agents/*, goals, landmarks, m_radius*/);
	//environment = new Environment(agents, num_goals, num_landmarks, max_radius);

	PI_angular_error.reset();
	printf("Run = %6u\tPI error = %6g +- %6g\n", int(PI_angular_error.count()), PI_angular_error.mean(), PI_angular_error.stddev());
	for(int run = 0; run < total_runs; run++){
		double start_time = controller->t;
		double old_gvl = controller->gln->length;		//Old GV len
		while(controller->t < start_time + max_outbound_time && environment->sum_reward < 1.){																	//OUTBOUND RUN (SEARCHING)
			run_outbound();
		}
		PI_angular_error(abs(bound_angle(controller->get_HV() - environment->get_real_HV())));
		PI_resultseach << param << "\t" << abs(bound_angle(controller->get_HV() - environment->get_real_HV())) << endl;
		//if((run+1)%(total_runs/100)==0)
			//printf("%1.6f\n", abs(bound_angle(controller->get_HV() - environment->get_real_HV())));
		if((run+1)%(total_runs/10)==0)
			printf("Run = %6u\tPI error = %6g +- %6g\t(%6g)\n", int(PI_angular_error.count()), PI_angular_error.mean(), PI_angular_error.stddev(), abs(bound_angle(controller->get_HV() - environment->get_real_HV())));
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
		//printf("Run = %3u, time@nest = %5u, hits/run = %2.3f, hits = %u, homing rate = %1.3f, dist_factor = %2.4f, expl_fact = %f, memory len = %1.3f\n", run+1, controller->t, (1.0*num_goalhits)/(1.0*(run+1)), num_goalhits, 1.0*num_homing/(run+1), controller->pin->length/environment->agent_list.at(0)->distance, controller->expl_factor, controller->pin->memory_length);
		environment->reset();
		controller->reset();
	}

	printf("Param = %6g\tPI error = %6g +- %6g\n\n", param, PI_angular_error.mean(), PI_angular_error.stddev());
	PI_results << param << "\t" << PI_angular_error.mean() << "\t" << PI_angular_error.stddev() << endl;
}

void Simulation::run_outbound(){
	controller->set_outbound();
	environment->mode = 0;
	environment->update(motor_command);
	controller->get_pos(environment->getx(), environment->gety());
	motor_command = controller->update(environment->agent_list.at(0)->phi, environment->agent_list.at(0)->v, environment->reward, environment->lm_recogn);
	distor << environment->agent_list.at(0)->distance << " " << controller->pin->length << " " << controller->gln->length << endl;
	//if(controller->t%500==0)
		//printf("t = %4u\tPI_error_ang = %1.3f\tSumR = %2.3f\tValue = %2.3f\tPhi = %3.2f\tGV angle = %3.2f (%2.3f)\tLV angle = %3.2f (%2.3f)\n", controller->t, PI_angular_error, environment->sum_reward, controller->value, in_degr(environment->agent_list.at(0)->phi), in_degr(controller->GV_angle), controller->gln->length, in_degr(controller->LV_angle), controller->rln->length);
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

string Simulation::get_param_type(param_types input){
	switch(input){
	case number_of_neurons:
		return "number_of_neurons";
		break;
	case sensory_noise:
		return "sensory_noise";
		break;
	case maximum_outbound_time:
		return "maximum_outbound_time";
		break;
	case exploration_factor:
		return "exploration_factor";
		break;
	case memory_leakage:
		return "memory_leakage";
		break;
	case response_function:
		return "response_function";
		break;
	case decoding_kernel:
		return "decoding_kernel";
		break;
	case PI_steer_constant:
		return "PI_steer_constant";
		break;
	case env_goal_density:
		return "env_goal_density";
		break;
	case learning_rate:
		return "learning_rate";
		break;
	}
	printf("Error. Didn't match keys.");
	return "";
}

string Simulation::get_param_name(param_types input){
	switch(input){
	case number_of_neurons:
		return "number of neurons";
		break;
	case sensory_noise:
		return "sensory noise";
		break;
	case maximum_outbound_time:
		return "maximum outbound time";
		break;
	case exploration_factor:
		return "exploration factor";
		break;
	case memory_leakage:
		return "memory leakage";
		break;
	case response_function:
		return "response function";
		break;
	case decoding_kernel:
		return "decoding kernel";
		break;
	case PI_steer_constant:
		return "PI steering constant";
		break;
	case env_goal_density:
		return "goal density";
		break;
	case learning_rate:
		return "learning rate";
		break;
	}
	printf("Error. Didn't match keys.");
	return "";
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
