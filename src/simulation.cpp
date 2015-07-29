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

	environment = (rand_env ? new Environment(ngs, nlms, m_rad, agents) : new Environment(agents));

	trial = 1;
	global_t = 0.0;
	trial_t = 0.0;
	timestep = 0;
	count_home = 0;
	avg_error = 0.0;
	travg_error = 0.0;

	agent_str.open("data/agent.dat");
	endpts_str.open("data/endpoints.dat");
}

Simulation::~Simulation(){
	agent_str.close();
	endpts_str.close();
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
		a(i)->init(control);
		controllers.push_back(control);
	}
}

void Simulation::reset(){
	environment->reset();
}

void Simulation::run(int in_numtrials, double in_duration, double in_interval){
	T = in_duration;
	dt = in_interval;
	global_t = 0.0;

	for(; trial < in_numtrials+1; trial++){
		reset();
		timestep = 0;
		trial_t = 0.;
		avg_error = 0.;
		while(trial_t < T){
			writeTrialData();
			timestep++;
			trial_t += dt;
			global_t += dt;

			update();
			//printf("t = %f\toutput = %f\n", timestep*dt, a(0)->dphi());
			if(a(0)->in())
				avg_error += abs(a(0)->HV(2)-a(0)->th());
		}
		writeSimData();
		avg_error /= (timestep-(T/(2*dt)));
		travg_error += avg_error;
		if(trial%100==0)
			printf("Trial = %u\tHoming success rate = %1.5f\tTotal error = %3.3f\tTrial error = %3.3f\n", trial, 1.0*count_home/(1.0*(trial)), 180.*(travg_error/trial)/M_PI, 180.*avg_error/M_PI);
	}
	travg_error /= in_numtrials;
}

void Simulation::update(){
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
	for(unsigned int i= 0; i< agents; i++){
		agent_str << "\t" << a(i)->x()<< "\t" << a(i)->y();
	}
	agent_str << endl;
}

