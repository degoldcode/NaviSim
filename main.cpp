/*
 * main.cpp
 *
 *  Created on: 01.08.2014
 *      Author: meicron
 */

#include <armadillo>
#include <fstream>
#include "environment.h"
#include "navicontrol.h"
#include "timer.h"
using namespace std;
using namespace arma;

NaviControl* controller;
Environment* environment;

ofstream distor;
ofstream gvlearn;
ofstream lvlearn;

const int num_neurons = 360;			//Number of array neurons
const int num_motivs = 2;				//0=outbound,	1=inbound
const int max_outbound_time = 1000;
const int max_inbound_time = 1000;//600;
const int total_runs = 500;
const double factor = 0.5;

const int agents = 1;
const double m_radius = 25.;
const double g_density = 0.02;//0.005;//0.0001;
const int goals = int(g_density * (M_PI * m_radius * m_radius));
const double lm_density = 0.0;//0.02;//0.001;// 0.0001;
const int landmarks = int(lm_density * (M_PI * m_radius * m_radius));
double command = 0.0;
double dreward = 0.0;
double PI_angle_error;
double PI_dist_error;
int total_hits = 0;
int total_homes = 0;

bool inbound_on = true;

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

double in_degr(double angle){
	return 180.*angle/M_PI;
}

int main(){
	gvlearn.open("./data/gvlearn.dat");
	distor.open("./data/distor.dat");
	Timer timer(true);
	controller = new NaviControl(num_neurons);											/////	+
	environment = new Environment(agents, goals, landmarks, m_radius);

	//environment->add_pipe(0.,0.,-0.0,10.,.2);			//Goal Learning Exp A (Pipe 1)
	//environment->add_pipe(0.,0.,-0.0,3.,.2);			//Goal Learning Exp A (Pipe 2)
	//double a = sqrt(2.)*3.;
	//double b = 10;//sqrt(75.);
	//environment->add_pipe(0., b, 0., 0., .15);
	//environment->add_pipe(b, b, 0., 10., .15);
	//environment->add_pipe(b, 0., 10., 10., .15);
	//environment->add_pipe(0., a, 0., a,.2);			//Goal Learning Exp A (Pipe 3)
	//environment->add_pipe(0.,25.,25.,25.,.2);
	//environment->add_pipe(0.2,-25.,25.,25.,.4);
	//environment->add_pipe(0.,0.,25.,50.,.4);
	environment->add_goal(0.,10.);
	//environment->add_goal(-25.,25.);
	//environment->add_goal(0.,50.);

	for(int run = 0; run < total_runs; run++){
		//if(total_runs > 100)
			//controller->reset_matrices();
		double start_time = controller->t;
		double old_gvl = controller->gln->length;		//Old GV len

		while(controller->t < start_time + max_outbound_time && environment->sum_reward < 1.){																	//OUTBOUND RUN (SEARCHING)
			controller->set_outbound();
			environment->mode = 0;
			dreward = -environment->reward;
			environment->update(command);
			dreward += environment->reward;
			command = controller->update(environment->agent_list.at(0)->phi, environment->agent_list.at(0)->v, environment->reward);
			distor << environment->agent_list.at(0)->distance << " " << controller->pin->length << " " << controller->gln->length << endl;
			PI_angle_error = bound_angle(controller->PI_avg_angle - environment->agent_list.at(0)->theta);
			PI_dist_error = controller->pin->length - environment->agent_list.at(0)->distance;
			if(controller->t%500==0)
				printf("t = %4u\tPI_error_ang = %1.3f\tSumR = %2.3f\tAccR = %2.3f\tPhi = %3.2f\tGV angle = %3.2f (%2.3f)\n", controller->t, PI_angle_error, environment->sum_reward, controller->accu_reward, in_degr(environment->agent_list.at(0)->phi), in_degr(controller->GV_angle), controller->gln->length);
		}
		double in_time = controller->t;
		while(inbound_on && environment->agent_list.at(0)->distance > 0.2 && controller->t < in_time + max_inbound_time){ 	//INBOUND RUN (PI HOMING)
			controller->set_inbound();
			environment->mode = 1;
			command = controller->update(environment->agent_list.at(0)->phi, environment->agent_list.at(0)->v, environment->reward);;
			environment->update(command);
			if(environment->agent_list.at(0)->in_pipe)
				cout << "No. I'm in a pipe.\n";
			//if(controller->t%100==0)
				//printf("t = %4u\tFB_error = %1.3f\tPhi = %1.3f\n", controller->t, controller->feedback_error, environment->agent_list.at(0)->phi);
		}
		//if(controller->gln->length - old_gvl < 0.000000001 && run > 20)	//convergence criterion
			//run = total_runs;
		if(environment->agent_list.at(0)->distance <= 0.2)
			total_homes++;
		total_hits += environment->get_hits();
		gvlearn << run << " " << controller->t << " " << 1.0*total_hits/(run+1) << " " << 1.0*total_homes/(run+1) << " " << controller->expl_factor << endl;
		printf("Run = %3u, time@nest = %5u, hits/run = %2.3f, hits = %u, homing rate = %1.3f, dist_factor = %2.4f, expl_fact = %f, memory len = %1.3f\n", run+1, controller->t, (1.0*total_hits)/(1.0*(run+1)), total_hits, 1.0*total_homes/(run+1), controller->pin->length/environment->agent_list.at(0)->distance, controller->expl_factor, controller->pin->memory_length);
		environment->reset();
		controller->reset();
	}
	controller->save_matrices();
	//printf("PI distortion rate = %4.9f, GV len = %4.9f\n", controller->pin->length/environment->agent_list.at(0)->distance, controller->gln->length);
	delete controller;
	delete environment;
	gvlearn.close();
	distor.close();
	auto elapsed_secs_cl = timer.Elapsed();
	printf("%4.3f s. Done.\n", elapsed_secs_cl.count()/1000.);
}


