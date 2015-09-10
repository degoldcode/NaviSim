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
	lvlearn_on = false;

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
	count_home = 0;
	count_goal = 0;

	agent_str.open("data/agent.dat");
	endpts_str.open("data/endpoints.dat");
	homevector_str.open("data/homevector.dat");
	globalvector_str.open("data/globalvector.dat");
	localvector_str.open("data/localvector.dat");
	refvector_str.open("data/refvector.dat");
	reward_str.open("data/reward.dat");
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
	globalvector_str.close();
	localvector_str.close();
	refvector_str.close();
	reward_str.close();
	sim_cfg.close();
	length_scaling.close();
	delete environment;
}

Agent* Simulation::a(int i){
	return environment->a(i);
}

void Simulation::add_goal(double x, double y, int color, double size, bool decay){
	environment->add_goal(x, y, color, size, decay);
}

void Simulation::add_goal(double max_radius){
	environment->add_goal(max_radius);
}

void Simulation::add_landmark(double x, double y){
	environment->add_landmark(x, y);
}

void Simulation::add_pipe(double x0, double x1, double y0, double y1){
	environment->add_pipe(x0,x1,y0,y1);
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
		control->set_sample_int(size/10);      // sample activity data every 10 time steps

		a(i)->init(control);
		controllers.push_back(control);
	}
}

void Simulation::lvlearn(bool _opt){
	lvlearn_on = _opt;
	c()->lvlearn_on = _opt;
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
	int total_steps = int(N*T/dt);
	sample_time = int(total_steps/1000000.);
	if(sample_time < 1)
		sample_time = 1;
	printf("Total timesteps is %u\nSet sampling interval to %u\n", total_steps, sample_time);

	if(c()->get_inward() == 0)
		c()->set_inward(T/dt);

	for(; trial < N+1; trial++){
		reset();
		while(trial_t < T){
			if(int(trial_t/dt)%sample_time == 0)
				writeTrialData();
			update();
			pi_error( (a(0)->HV()-a(0)->v()).len() );
		}
		if(trial_t <= T + 0.5)
			is_home(0);

		if(N > 1){
			total_pi_error( pi_error.mean() );
			avg_length(a(0)->d());
			writeSimData();
			if(N > 19 && trial%(in_numtrials/20)==0){
				printf("#%u\t", trial);
				if(pin_on)
					printf("e=%2.3f\t<e>=%2.3f\t", pi_error.mean(), total_pi_error.mean());
				if(homing_on)
					printf("<G>=%5.0f\t", 1.0*count_goal);
					//printf("<Home>=%3.1f%%\t<Goal>=%2.0f\t", 100.*is_home.mean(), 1.0*count_goal);
				if(gvlearn_on)
					printf("e= %1.2f\tGV= (%3.1f, %1.2f)\t", c()->expl(0), a(0)->GV().ang().deg(), a(0)->GV().len());
				if(lvlearn_on){
					for(int i = 0; i < c()->K(); i++){
						printf("LV%u=(%3.1f,%1.2f), V%u= %1.1f\t", i, c()->LV(i).ang().deg(), c()->LV(i).len(), i, c()->LV_value_raw(i));
					}
				}
				//printf("Amount=%g", e()->g(0)->a());
				printf("\n");
			}

			if(N <= 19){
				if(pin_on)
					printf("#%u\te=%2.3f\t<e>=%2.3f\t", trial, pi_error.mean(), total_pi_error.mean());
				if(homing_on)
					printf("<Home>=%3.1f%%\t<Goal>=%2.4f\t", 100.*is_home.mean(), is_goal.mean());
				if(gvlearn_on)
					printf("Expl=%1.5f\tGV(ang,r)=(%3.2f,%2.3f)\tGVC(ang,r)=(%3.2f,%2.3f)", c()->expl(0), a(0)->GV().ang().deg(), a(0)->GV().len(), c()->GVc().ang().deg(), c()->GVc().len());
				if(lvlearn_on)
					printf("LV(ang,r)=(%3.2f,%2.3f)\t", c()->LV().ang().deg(), c()->LV().len());
				printf("\n");
			}

//				printf("Trial = %u\tTrial R = %g\tTotal R = %g\n", trial, e()->get_trial_r(), e()->get_total_r());
				//printf("Trial = %u\tAvg Length = %1.5f\tVar Length = %3.3f\n", trial, avg_length.max(), avg_length.var());
		}

	}
}

void Simulation::set_inward(int _time){
	c()->set_inward(_time);
}

void Simulation::update(){
//	if(accu(c()->GV_module()->dW()) < 0.0 && (a(0)->pos - c()->HV()).len() > 0.3)
//		printf("GV learn at (%g,%g) -> (%g, %g), R = %g\n", a(0)->pos.x, a(0)->pos.y, c()->HV().x, c()->HV().y, c()->GV_module()->R());

	if(timestep%1000==0 && N == 1 && pin_on)
		printf("Time = %g\te = %g\te_max = %g\n", trial_t, pi_error.mean(), pi_error_max.mean());
	timestep++;
	trial_t += dt;
	global_t += dt;
	environment->update();
	is_goal(environment->get_hits(0));
	count_goal += environment->get_hits(0);
	if(environment->stop_trial){
		if(N < 10)
			printf("Homing success at %g s\n", trial_t);
		trial_t = T+1.;
		count_home++;
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
	agent_str << trial << "\t" << trial_t;						//1,2
	agent_str << "\t" << a(0)->x()<< "\t" << a(0)->y();			//3,4
	agent_str << "\t" << a(0)->d() << "\t" << a(0)->dphi();		//5,6
	agent_str << "\t" << a(0)->v().ang() << "\t" << global_t;	//7,8
	if(lvlearn_on)
		agent_str << "\t" << e()->lmr(0)(0) << "\t" << c()->el_lm(0) << "\t" << c()->el_lm(1) << "\t" << c()->gl_w << "\t" << c()->expl(0); // TODO: different streams for different agents
	agent_str << endl;
	if(pin_on){
		homevector_str << trial_t << "\t" << global_t;
		homevector_str << "\t" << a(0)->HV().x << "\t" << a(0)->HV().y << "\t" << a(0)->HVm().x << "\t" << a(0)->HVm().y << "\t" << a(0)->HV().ang() << "\t" << a(0)->HVm().ang() << "\t" <<  (a(0)->HV()-a(0)->v()).len() << "\t" <<  a(0)->HV().len() << endl;
	}
	if(gvlearn_on){
		globalvector_str << trial_t << "\t" << global_t;
		globalvector_str << "\t" << a(0)->GV().x << "\t" << a(0)->GV().y;
		globalvector_str << "\t" << a(0)->GV().ang() << "\t" << a(0)->GV().len();
		globalvector_str << "\t" << c()->expl(0) << "\t" << 1.0*count_goal;
		globalvector_str << "\t" << c()->GV_vecavg() << endl;
	}
	if(lvlearn_on){
		refvector_str << trial_t << "\t" << global_t;
		refvector_str << "\t" << c()->RV().x << "\t" << c()->RV().y << "\t" << c()->RV().ang() << "\t" << c()->RV().len() << endl;
		localvector_str << trial_t << "\t" << global_t;
		localvector_str << "\t" << c()->LV(0).x << "\t" << c()->LV(0).y << "\t" << c()->LV(0).ang() << "\t" << c()->LV(0).len();
		localvector_str << "\t" << c()->LV(1).x << "\t" << c()->LV(1).y << "\t" << c()->LV(1).ang() << "\t" << c()->LV(1).len() << endl;
	}
	reward_str << trial_t << "\t" << global_t;
	reward_str << "\t" << c()->R(0) << "\t" << c()->v(0) << endl;
	length_scaling << a(0)->v().len() << "\t" << sum(a(0)->pi()->get_output()) << "\t" << a(0)->c()->N() << endl;
}

