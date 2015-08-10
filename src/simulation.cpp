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

Simulation::Simulation(int in_numtrials, int in_agents, bool random_env){
	N = in_numtrials;
	agents = in_agents;
	rand_env = random_env;
	VERBOSE = false;
	SILENT = false;

	pin_on = true;
	homing_on = false;
	gvlearn_on = false;
	gvnavi_on = false;

	(VERBOSE)?printf("Building environment.\n"):VERBOSE;
	//environment = (rand_env ? new Environment(10, 10, 25., 1) : new Environment(agents));
	environment = (rand_env ? new Environment(ngs, nlms, m_rad, agents) : new Environment(agents));
	(VERBOSE)?printf("Done.\n"):VERBOSE;

	T = 0.;
	dt = 0.;
	trial = 1;
	global_t = 0.;
	trial_t = 0.;
	timestep = 0;

	agent_str.open("data/agent.dat");
	endpts_str.open("data/endpoints.dat");
	homevector_str.open("data/homevector.dat");
	sim_cfg.open("data/sim.cfg");
	sim_cfg << "# Na\t# Nn\t# Sno\t# Leak\t# Uncno" << endl;
	sim_cfg << agents << "\t";
	length_scaling.open("data/l_scale.dat");
}

Simulation::~Simulation(){
	total_pi_error.reset();
	agent_str.close();
	endpts_str.close();
	homevector_str.close();
	sim_cfg.close();
	length_scaling.close();
	delete environment;
}

Agent* Simulation::a(int i){
	return environment->a(i);
}

void Simulation::add_goal(double x, double y, int color){
	environment->add_goal(x, y, color);
}

void Simulation::add_goal(double max_radius){
	environment->add_goal(max_radius);
}

Controller* Simulation::c(int i){
	return controllers.at(i);
}

Environment* Simulation::e(){
	return environment;
}

void Simulation::gvlearn(bool _opt){
	gvlearn_on = _opt;
	c()->gvlearn_on = _opt;
}

void Simulation::homing(bool _opt){
	homing_on = _opt;
	c()->homing_on = _opt;
}

void Simulation::init_controller(int num_neurons, double sensory_noise, double uncor_noise, double leakage){
	sim_cfg << num_neurons << "\t" << sensory_noise << "\t" << uncor_noise << "\t" << leakage << endl;
	for(unsigned int i= 0; i< agents; i++){
		Controller* control = new Controller(num_neurons, sensory_noise, leakage, uncor_noise);
		control->SILENT = SILENT;
		int size = N*pow( 10, int(log10( double( num_neurons ) ) ) );
		control->set_sample_int(size);      // sample activity data every 10 time steps

		a(i)->init(control);
		controllers.push_back(control);
	}
}

void Simulation::reset(){
	timestep = 0;
	trial_t = 0.;
	pi_error.reset();
	pi_error_max.reset();
	environment->reset();
}

void Simulation::run(int in_numtrials, double in_duration, double in_interval){
	N = in_numtrials;
	T = in_duration;
	dt = in_interval;
	global_t = 0.0;

	for(; trial < N+1; trial++){
		reset();
		while(trial_t < T){
			writeTrialData();
			update();
			pi_error( (a(0)->HV()-a(0)->v()).len() );
		}
		if(trial_t == T)
			is_home(0);

		if(N > 1){
			total_pi_error( pi_error.mean() );
			avg_length(a(0)->d());
			writeSimData();
			if(N > 9 && trial%(in_numtrials/10)==0){
				if(pin_on)
					printf("Trial = %u\te = %g\t<e> = %g\t", trial, pi_error.mean(), total_pi_error.mean());
				if(homing_on)
					printf("<Homing> = %g\t", is_home.mean());
				//if(gvlearn_on)
					//printf("Trial = %u\te = %g\t<e> = %g", trial, pi_error.mean(), total_pi_error.mean());
			}

			if(N <= 9){
				if(pin_on)
					printf("Trial = %u\te = %g\t<e> = %g", trial, pi_error.mean(), total_pi_error.mean());
				if(homing_on)
					printf("<Homing> = %g\t", is_home.mean());
				//if(gvlearn_on)
					//printf("Trial = %u\te = %g\t<e> = %g", trial, pi_error.mean(), total_pi_error.mean());
			}
			printf("\n");
//				printf("Trial = %u\tTrial R = %g\tTotal R = %g\n", trial, e()->get_trial_r(), e()->get_total_r());
				//printf("Trial = %u\tAvg Length = %1.5f\tVar Length = %3.3f\n", trial, avg_length.max(), avg_length.var());
		}

	}
}

void Simulation::set_inward(int _time){
	c()->set_inward(_time);
}

void Simulation::update(){
	if(timestep%1000==0 && N == 1)
		printf("Time = %g\te = %g\te_max = %g\n", trial_t, pi_error.mean(), pi_error_max.mean());
	timestep++;
	trial_t += dt;
	global_t += dt;
	environment->update();
	if(environment->stop_trial){
		if(N < 10)
			printf("Homing success at %g s\n", trial_t);
		trial_t = T+1.;
		is_home(1);
	}
}

void Simulation::writeSimData(){
	endpts_str << trial;
	for(unsigned int i= 0; i< agents; i++){
		endpts_str << "\t" << a(i)->x()<< "\t" << a(i)->y() << "\t" << a(i)->d();
	}
	endpts_str << endl;
}

void Simulation::writeTrialData(){
	agent_str << trial << "\t" << trial_t;
	agent_str << "\t" << a(0)->x()<< "\t" << a(0)->y() << "\t" << a(0)->d() << "\t" << a(0)->dphi()<< "\t" << a(0)->v().ang() << "\t" << global_t ; // TODO: different streams for different agents
	agent_str << endl;
	homevector_str << trial_t << "\t" << global_t;
	homevector_str << "\t" << a(0)->HV().x << "\t" << a(0)->HV().y << "\t" << a(0)->HVm().x << "\t" << a(0)->HVm().y << "\t" << a(0)->HV().ang() << "\t" << a(0)->HVm().ang() << "\t" <<  (a(0)->HV()-a(0)->v()).len() << "\t" <<  a(0)->HV().len() << endl;
	length_scaling << a(0)->v().len() << "\t" << sum(a(0)->pi()->get_output()) << "\t" << a(0)->c()->N() << endl;
}

