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

Simulation::Simulation(int in_agents, bool random_env){
	agents = in_agents;
	rand_env = random_env;
	VERBOSE = false;

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
}

Simulation::~Simulation(){
	agent_str.close();
	endpts_str.close();
	homevector_str.close();
	delete environment;
}

Agent* Simulation::a(int i){
	return environment->a(i);
}

Environment* Simulation::e(){
	return environment;
}

void Simulation::init_controller(int num_neurons, double sensory_noise, double leakage){
	for(unsigned int i= 0; i< agents; i++){
		Controller* control = new Controller(num_neurons, sensory_noise, leakage);
		control->set_sample_int(10);      // sample activity data every 10 time steps
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
	T = in_duration;
	dt = in_interval;
	global_t = 0.0;

	for(; trial < in_numtrials+1; trial++){
		reset();
		while(trial_t < T){
			writeTrialData();
			update();

			//if(a(0)->in())
				pi_error( (a(0)->HV()-a(0)->v()).len() );
				pi_error_max( (a(0)->HVm()-a(0)->v()).len() );
		}

		if(in_numtrials > 1){
			total_pi_error( pi_error.mean() );
			avg_length(a(0)->d());
			writeSimData();
			if(in_numtrials > 9 && trial%(in_numtrials/10)==0)
				printf("Trial = %u\tTrial R = %g\tTotal R = %g\n", trial, e()->get_trial_r(), e()->get_total_r());
				//printf("Trial = %u\tAvg Length = %1.5f\tVar Length = %3.3f\n", trial, avg_length.max(), avg_length.var());
		}

	}
}

void Simulation::update(){
	if(timestep%1000==0)
		printf("time = %g\te = %g\te_max = %g\n", trial_t, pi_error.mean(), pi_error_max.mean());
	timestep++;
	trial_t += dt;
	global_t += dt;
	environment->update();
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
	agent_str << "\t" << a(0)->x()<< "\t" << a(0)->y() << "\t" << a(0)->d() << "\t" << a(0)->dphi()<< "\t" << a(0)->v().ang() ; // TODO: different streams for different agents
	agent_str << endl;
	homevector_str << trial << "\t" << trial_t;
	homevector_str << "\t" << a(0)->HVm().x << "\t" << a(0)->HVm().y << "\t" << a(0)->HV().ang() << "\t" << a(0)->HVm().ang() << endl;
}

