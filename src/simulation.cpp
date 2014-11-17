/*****************************************************************************
 *  simulation.cpp                                                           *
 *                                                                           *
 *  Created on:   Aug 31, 2014                                               *
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

#include "simulation.h"

Simulation::Simulation(string in_param_type, int in_num_trials){
	file_name.str(string());
	file_name << "./data/PIerror_" << in_param_type << ".dat";

	gv_angl.open("./data/gv_angl.dat");
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
	if(run_div<1)
		run_div=1;
	//printf("runs= %u - > div = %u\n", total_runs, run_div);
	file_ext = in_param_type;

	// Evaluation
	success_rate_avg.resize(total_runs);
	explor_rate_avg.resize(total_runs);
	success_rate_avg2.resize(total_runs);
	explor_rate_avg2.resize(total_runs);
	prob_B.resize(total_runs);
	prob_Y.resize(total_runs);
	choice.resize(total_runs);

	controller = new NaviControl(num_neurons, sens_noise, pi_leakage);
	environment = (rand_env ? new Environment(ngs, nlms, m_rad, agents) : new Environment(agents));
	global_time = 0.0;
	timestep = 0;
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
	gv_angl.close();
}

double Simulation::bound(double angle){
	double rphi = angle;
	while(rphi > M_PI)
		rphi -= 2 * M_PI;
	while(rphi < - M_PI)
		rphi += 2 * M_PI;
	return rphi;
}

NaviControl* Simulation::get_controller(){
	return controller;
}

Environment* Simulation::get_environment(){
	return environment;
}

int Simulation::get_lrun(){
	return last_run;
}

double Simulation::get_srate(){
	return success_rate;
}

double Simulation::in_degr(double angle){
	if(angle > 0.)
		return 180.*angle/M_PI;
	else
		return 180.*angle/M_PI + 360.;
}

double Simulation::inv_angle(double angle){
	return bound(angle - M_PI);
}

void Simulation::reset(){
	//************ reset trial-based statistics ************//
	PI_angular_error.reset();
	num_goalhits = 0;
	last_run = 0;
	blue_hits = 0;
	yellow_hits = 0;

	//************ reset controller and environment ************//
	delete controller;
	delete environment;
	controller = new NaviControl(num_neurons, sens_noise, pi_leakage);
	//printf("set param -> %f\n", controller->pin->snoise);
	environment = (rand_env ? new Environment(ngs, nlms, m_rad, agents) : new Environment(agents));

	//************ Put Your Objects Here ************//

	//environment->add_pipe(0.0,3.0,0.0,3.0, 0.15);
	//environment->add_pipe(0.0,0.0,0.0,5.0, 0.15);
	environment->add_pipe(0.,0.,0.,-5.);
	environment->add_pipe(0.,-5.,-5.,-5.);
	environment->add_goal(-5., -5., 0);
	//environment->add_goal(-1.5,3.,0);
	//environment->add_goal(1.5,3.,1);
}

void Simulation::run(){
	reset();

	//************ Start (learning/parameter) cycle ************//
	for(int run = 0; run < total_runs; run++){
		/*if(total_runs > 50 && run==int(total_runs/4))
			controller->beta = 1.;*/
		if(total_runs > 50 && run==int(total_runs/2)){
			environment->swap_reward();
		}

		double start_time = global_time;
		double trial_time = 0.0;

		//************ Outbound run ************//

		while(global_time < start_time + max_outbound_time && environment->get_sum_reward() < 0.25){																	//OUTBOUND RUN (SEARCHING)
			run_outbound();
			endpoints << controller->HV()->x()-environment->x() << "\t" << controller->HV()->y()-environment->y() << endl;
			trial_time+=dt;
			global_time+=dt;
		}
		PI_angular_error(abs(bound(controller->HV()->avg() - environment->th())));
		PI_resultseach << abs(bound(controller->HV()->avg() - environment->th())) << endl;



		double in_time = global_time;

		//************ Inbound run ************//

		while(inbound_on && environment->agent()->d() > 0.2 && global_time < in_time + max_inbound_time){ 	//INBOUND RUN (PI HOMING)
			run_inbound();
			endpoints << controller->HV()->x()-environment->x() << "\t" << controller->HV()->y()-environment->y() << endl;
			trial_time+=dt;
			global_time+=dt;
		}

		//************ Console output ************//

		if((run+1)%run_div==0 && environment->n_goals() > 0){
			printf("Run = %5u\t", run+1);
			printf("Success rate: %3.3f,\t", success_rate);
			printf("Expl. rate: %1.5e,\n", controller->expl());
			for(int i = 0; i < num_motivs; i++){
				printf("GV = (%3.2f", controller->GV(i)->x());
				printf(",%3.2f) ", controller->GV(i)->y());
				printf("angle = %3.3f", controller->GV(i)->avg());
				printf(" (%3.3f),\t", in_degr(controller->GV(i)->avg()));
				printf("nearest (%3.2f,", environment->nearest(controller->GV(i)->x(), controller->GV(i)->y())->x());
				printf("%3.2f) ", environment->nearest(controller->GV(i)->x(), controller->GV(i)->y())->y());
				printf("has amount = %g\n", environment->nearest(controller->GV(i)->x(), controller->GV(i)->x())->a());
			}
			printf("P[B] = %f\t P[Y] = %f\t-> %u\n", controller->p(0), controller->p(1), controller->q());
		}

		//************ Evaluation of cycle ************//

		if(environment->agent()->d() <= 0.2)
			num_homing++;
		if(environment->get_hits() > 0)
			num_goalhits ++;

		success_rate = 100.0*num_goalhits/(run+1);
		if(environment->get_hits(0) > 0)
			blue_hits++;
		if(environment->get_hits(1) > 0)
			yellow_hits++;

		success_rate_avg.at(run)(blue_hits);
		explor_rate_avg.at(run)(controller->expl(0));
		success_rate_avg2.at(run)(yellow_hits);
		explor_rate_avg2.at(run)(controller->expl(1));
		prob_B.at(run)(controller->p(0));
		prob_Y.at(run)(controller->p(1));
		choice.at(run)(controller->q());

		if(environment->n_goals() > 0){
			gvlearn << run << " " << global_time << " " << success_rate/100. << " " << 1.0*num_homing/(run+1) << " " << controller->expl() << endl;
			gv_history0 = join_rows(gv_history0, controller->GV(0)->w().col(0));
			gv_history1 = join_rows(gv_history1, controller->GV(1)->w().col(0));
			gv_angl << run << "\t" << controller->GV(0)->avg() << "\t" << controller->GV(1)->avg() << endl;
		}
		environment->reset();
		controller->reset();

		if(success_rate > 95. && controller->expl() < 0.05 && last_run == 0){
			printf("Converged after %u trials. Avg success rate = %3.3f. Exploration rate = %1.9f.\n", run+1, success_rate, controller->expl());
			//run = total_runs;
			last_run = run+1;
		}
		if(environment->n_goals() > 0){
		statsall_gl << run
					<< "\t" << last_run
					<< "\t" << controller->expl()
					<< "\t" << 100.0*num_goalhits/total_runs
					<< "\t" << sqrt(pow(controller->GV(0)->x(),2)+pow(controller->GV(0)->y(),2))
					<< "\t" << sqrt(pow(controller->GV(1)->x(),2)+pow(controller->GV(1)->y(),2))
					<< "\t" << sqrt(pow(environment->nearest()->x(),2)+pow(environment->nearest()->y(),2))
					<< endl;
		}
		if(run+1==total_runs && last_run == 0)
			last_run = total_runs;
	}
	if(last_run == total_runs && environment->n_goals() > 0){
		printf("No convergence after %u.\nNearest goal at (%3.2f,%3.2f) = %3.2f\n\n", total_runs, environment->nearest()->x(), environment->nearest()->y(), sqrt(pow(environment->nearest()->x(),2)+pow(environment->nearest()->y(),2)));
	}

	total_reward(environment->get_total_reward());
	gv_history0.save("./data/gv_hist0.mat", raw_ascii);
	gv_history1.save("./data/gv_hist1.mat", raw_ascii);
	if(environment->n_goals() > 0){
		stats_gl <<	last_run
				<< "\t" << controller->expl()
				<< "\t" << 100.0*num_goalhits/total_runs
				<< "\t" << sqrt(pow(controller->GV(0)->x(),2)+pow(controller->GV(0)->y(),2))
				<< "\t" << sqrt(pow(controller->GV(1)->x(),2)+pow(controller->GV(1)->y(),2))
				<< "\t" << sqrt(pow(environment->nearest()->x(),2)+pow(environment->nearest()->y(),2))
				<< endl;
	}
}

void Simulation::run_outbound(){
	controller->set_mode(outbound);
	environment->set_mode(outbound);
	environment->update(motor_command);
	controller->get_pos(environment->x(), environment->y());
	motor_command = controller->update(environment->agent()->phi(), environment->agent()->v(), environment->r(), environment->color());
	distor << environment->agent()->d() << " " << controller->HV()->len() << " " << controller->GV(0)->len() << endl;
}

void Simulation::run_inbound(){
	controller->set_mode(inbound);
	environment->set_mode(inbound);
	motor_command = controller->update(environment->agent()->phi(), environment->agent()->v(), environment->r(), environment->lmr());;
	environment->update(motor_command);
}








