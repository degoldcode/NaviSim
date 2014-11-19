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

	agent_str.open("data/agent.dat");
}

Simulation::~Simulation(){
	delete environment;
}

Agent* Simulation::a(int i){
	return environment->a(i);
}

double Simulation::randn(double mean, double var){
	static random_device e{};
	static normal_distribution<double> d(mean, var);
	return d(e);
}

void Simulation::run(int in_numtrials, double in_duration, double in_interval){
	T = in_duration;
	dt = in_interval;
	int tstep = 0;
	for(int trial_num= 0; trial_num < in_numtrials; trial_num++){
		while(tstep*dt< T){
			agent_str << tstep*dt;
			update(randn(0.,1.));
			tstep++;
		}
	}
}

void Simulation::update(double command){
	for(unsigned int i= 0; i< agents; i++){
		agent_str << "\t" << a(i)->x()<< "\t" << a(i)->y();
		environment->update(command);
	}
	agent_str << endl;
}
