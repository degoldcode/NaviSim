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
	endpoints.open("./data/end.dat");
	PI_results.open(file_name.str().c_str());


	///*** PI EXP
	file_name.str(string());
	file_name << "./data/PIerroreach_" << in_param_type << ".dat";
	PI_resultseach.open(file_name.str().c_str());
	//lvlearn.open("./data/lvlearn.dat");

	///*** GOAL LEARNING EXP
	stats_gl.open("./data/gl_stats.dat");
	statsall_gl.open("./data/gl_statsall.dat");

	// Simulation parameters
	total_runs = in_num_trials;
	run_div = int(total_runs/5.);
	if(run_div<1)
		run_div=1;
	//printf("runs= %u - > div = %u\n", total_runs, run_div);
	inbound_on = false;
	num_params = 0;
	file_ext = in_param_type;

	//*** DEFAULT VALUES **//

	// Controller parameters
	num_neurons = 360;			//Number of array neurons
	sens_noise = 0.00;
	pi_leakage = 0.00;
	num_motivs = 2;				//0=outbound,	1=inbound
	max_outbound_time = 1000;
	max_inbound_time = max_outbound_time;//600;
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
	success_rate = 0.0;

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
	endpoints.close();
	stats_gl.close();
	statsall_gl.close();
}

void Simulation::run_sim(double param){
	delete controller;
	delete environment;

	controller = new NaviControl(num_neurons, sens_noise, pi_leakage);
	//printf("set param -> %f\n", controller->pin->snoise);
	//environment = new Environment(agents/*, goals, landmarks, m_radius*/);
	environment = new Environment(agents, num_goals, num_landmarks, max_radius);

	//environment->add_pipe(0.0,0.0,0.0,-5.0, 0.15);
	//environment->add_pipe(0.0,0.0,0.0,5.0, 0.15);
	//environment->add_goal(0.,3.);

	PI_angular_error.reset();
	num_goalhits = 0;
	end_run = 0;
	//printf("Run = %6u\tPI error = %6g +- %6g\n", int(PI_angular_error.count()), PI_angular_error.mean(), PI_angular_error.stddev());
	for(int run = 0; run < total_runs; run++){
		double start_time = controller->t;
		double trial_time = 0.0;
		double old_gvl = controller->gln->length;		//Old GV len
		while(controller->t < start_time + max_outbound_time && environment->sum_reward < 0.4){																	//OUTBOUND RUN (SEARCHING)
			run_outbound();
			endpoints << controller->PI_x-environment->getx() << "\t" << controller->PI_y-environment->gety() << endl;
			trial_time+=0.1;
		}
		PI_angular_error(abs(bound_angle(controller->get_HV() - environment->get_real_HV())));
		PI_resultseach << param << "\t" << abs(bound_angle(controller->get_HV() - environment->get_real_HV())) << endl;
		//if((run+1)%(total_runs/100)==0)
			//printf("%1.6f\n", abs(bound_angle(controller->get_HV() - environment->get_real_HV())));
		double in_time = controller->t;
		while(inbound_on && environment->agent_list.at(0)->distance > 0.2 && controller->t < in_time + max_inbound_time){ 	//INBOUND RUN (PI HOMING)
			run_inbound();
			endpoints << controller->PI_x-environment->getx() << "\t" << controller->PI_y-environment->gety() << endl;
			trial_time+=0.1;
		}
		success_rate = 100.0*num_goalhits/(run+1);
		if((run+1)%run_div==0)
			printf("Run = %6u\tPI error = %6g +- %6g\t(%6g),\tSuccess rate: %g,\tExpl. rate: %g\n", int(PI_angular_error.count()), PI_angular_error.mean(), PI_angular_error.stddev(), abs(bound_angle(controller->get_HV() - environment->get_real_HV())), success_rate, controller->expl_factor);
		//if(controller->gln->length - old_gvl < 0.000000001 && run > 20)	//convergence criterion
		//run = total_runs;
		if(environment->agent_list.at(0)->distance <= 0.2)
			num_homing++;
		if(environment->get_hits() > 0)
			num_goalhits ++;
		if(controller->expl_factor < 0.5)
			environment->agent_list.at(0)->short_write = false;
		gvlearn << run << " " << controller->t << " " << success_rate/100. << " " << 1.0*num_homing/(run+1) << " " << controller->expl_factor << endl;
		//printf("Run = %3u, time@nest = %5u, hits/run = %2.3f, hits = %u, homing rate = %1.3f, dist_factor = %2.4f, expl_fact = %f, memory len = %1.3f\n", run+1, controller->t, (1.0*num_goalhits)/(1.0*(run+1)), num_goalhits, 1.0*num_homing/(run+1), controller->pin->length/environment->agent_list.at(0)->distance, controller->expl_factor, controller->pin->memory_length);
		environment->reset();
		controller->reset();

		if(success_rate > 95. && controller->expl_factor < 0.05 && end_run == 0){
			printf("Converged after %u trials. Avg success rate = %3.3f. Exploration rate = %1.9f.\n", run+1, success_rate, controller->expl_factor);
			printf("Learned goal at (%3.2f,%3.2f) = %3.2f\nNearest goal at (%3.2f,%3.2f) = %3.2f\n\n", controller->GV_x, controller->GV_y, sqrt(pow(controller->GV_x,2)+pow(controller->GV_y,2)), environment->get_nearest_x(), environment->get_nearest_y(), sqrt(pow(environment->get_nearest_x(),2)+pow(environment->get_nearest_y(),2)));
			//run = total_runs;
			end_run = run+1;
		}
		statsall_gl << run
					<< "\t" << end_run
					<< "\t" << controller->expl_factor
					<< "\t" << 100.0*num_goalhits/total_runs
					<< "\t" << sqrt(pow(controller->GV_x,2)+pow(controller->GV_y,2))
					<< "\t" << sqrt(pow(environment->get_nearest_x(),2)+pow(environment->get_nearest_y(),2))
					<< endl;
		if(run+1==total_runs && end_run == 0)
			end_run = total_runs;
	}
	if(end_run == total_runs){
		printf("No convergence after %u.\nNearest goal at (%3.2f,%3.2f) = %3.2f\n\n", total_runs, environment->get_nearest_x(), environment->get_nearest_y(), sqrt(pow(environment->get_nearest_x(),2)+pow(environment->get_nearest_y(),2)));
	}
	stats_gl <<	end_run
			<< "\t" << controller->expl_factor
			<< "\t" << 100.0*num_goalhits/total_runs
			<< "\t" << sqrt(pow(controller->GV_x,2)+pow(controller->GV_y,2))
			<< "\t" << sqrt(pow(environment->get_nearest_x(),2)+pow(environment->get_nearest_y(),2))
			<< endl;
	//printf("Param = %6g\tPI error = %6g +- %6g\n\n", param, PI_angular_error.mean(), PI_angular_error.stddev());
	//PI_results << param << "\t" << PI_angular_error.mean() << "\t" << PI_angular_error.stddev() << endl;
}

void Simulation::run_outbound(){
	controller->set_outbound();
	environment->mode = 0;
	environment->update(motor_command);
	controller->get_pos(environment->getx(), environment->gety());
	//if(environment->reward>0.0)
		//printf("R=%2.3f, r(t)= %2.3f, (x,y)=(%2.3f,%2.3f)\n", environment->sum_reward, environment->reward,environment->getx(), environment->gety());
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
	//if(environment->agent_list.at(0)->in_pipe)
		//cout << "No. I'm in a pipe.\n";
	//if(controller->t%100==0)
	//printf("t = %4u\tFB_error = %1.3f\tPhi = %1.3f\n", controller->t, controller->feedback_error, environment->agent_list.at(0)->phi);
}

NaviControl* Simulation::get_controller(){
	return controller;
}

Environment* Simulation::get_environment(){
	return environment;
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
