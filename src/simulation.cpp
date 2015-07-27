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

	global_time = 0.0;
	trial_time = 0.0;
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

	for(int trial_num= 0; trial_num < in_numtrials; trial_num++){
		reset();
		int tstep = 0;
		avg_error = 0.;
		while(tstep*dt< T){
			agent_str << tstep*dt+trial_num*T;
			if(tstep*dt > T/2)
				a(0)->set_inward(true);
			//printf("t = %4.2f s\tinward = %d\tdistance = %2.3f\t(x,y) = (%2.3f,%2.3f)\t(xHV,yHV) = (%2.3f,%2.3f)\n",tstep*dt,a(0)->in(),a(0)->d(),a(0)->x(),a(0)->y(),a(0)->HV(0),a(0)->HV(1));
			if(a(0)->d()<0.2 && a(0)->in()){
				count_home++;
				break;
			}
			//double control_output =
			update();
			if(a(0)->in())
				avg_error += abs(a(0)->HV(2)-a(0)->th());
			tstep++;
		}
		avg_error /= (tstep-(T/(2*dt)));
		travg_error += avg_error;
		printf("Trial = %u\tHoming success rate = %1.5f\tTotal error = %3.3f\tTrial error = %3.3f\n", trial_num+1, 1.0*count_home/(1.0*(trial_num+1)), 180.*(travg_error/(trial_num+1))/M_PI, 180.*avg_error/M_PI);
		for(unsigned int i= 0; i< agents; i++){
			endpts_str << "\t" << a(i)->x()<< "\t" << a(i)->y();
		}
		endpts_str << endl;
	}
	travg_error /= in_numtrials;
}

void Simulation::update(){
	for(unsigned int i= 0; i< agents; i++){
		agent_str << "\t" << a(i)->x()<< "\t" << a(i)->y();
	}
	agent_str << endl;
	environment->update();
}
