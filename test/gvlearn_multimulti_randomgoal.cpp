/*
 * test_pi_single.cpp
 *
 *  Created on: 03.08.2015
 *      Author: meicron
 */

#include "../src/simulation.h"
#include "../src/timer.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
using namespace std;

Simulation* sim;
const int numagents= 1;
const int numtrials= 1000; //50;
const double T= 300.; //300,600,900,1200,1500
const double Thome= 200.; //200,400,600,800,1000
const double dt= 0.1;
const int numcycles = 1;

///
vector< running_stat<double> > expl_rate;
vector< running_stat<double> > home_rate;
vector< running_stat<double> > goal_rate;
double R_ratio;
running_stat<double> stat_ratio;
running_stat<double> trial_converge;
ofstream multi_cycletrials;
ofstream stat_cycles;

int main(){
	Timer timer(true);
	expl_rate.resize(numtrials);
	home_rate.resize(numtrials);
	goal_rate.resize(numtrials);
	multi_cycletrials.open("data/m_cycles_trials.dat");
	multi_cycletrials.width(10);
	multi_cycletrials << "#Trial\t#ExplMean\t#ExplSTD\t#HomeMean\t#HomeSTD\t#GoalMean\t#GoalSTD\n";
	stat_cycles.open("data/stat_cycles.dat");
	stat_cycles.width(12);
	stat_cycles << "#Cycle\t#RatioMean\t#RatioSTD\t#TimeMean\t#TimeSTD\t#ConvergRate\n";

	for(unsigned int cycle = 1; cycle < numcycles+1; cycle++){
		sim = new Simulation(numtrials, numagents, true);
		sim->SILENT = true;
		sim->homing(true);
		sim->gvlearn(true);
		sim->beta(true);
		sim->lvlearn(false);
		sim->init_controller(18, 1, 1, 0.05, 0.0, 0.00, 0.0);
		sim->set_inward(int(Thome/dt));							// inward after 1000 seconds
		sim->run(numtrials, T, dt);
		printf("Start cycle #%u\n", cycle);
		for(unsigned int trial = 0; trial < numtrials; trial++){
			if(trial%200==0)
				printf("Trial = %u\tExpl = %1.4f\tHome = %1.4f\tGoal = %1.4f\n", trial, sim->expl_rate.at(trial), sim->home_rate.at(trial), sim->goal_rate.at(trial));
			expl_rate.at(trial)(sim->expl_rate.at(trial));
			home_rate.at(trial)(sim->home_rate.at(trial));
			goal_rate.at(trial)(sim->goal_rate.at(trial));
		}

		R_ratio = sim->e()->nearest()->d()/sim->c(0)->GV(0).len();
		if(R_ratio > 1)
			R_ratio = 1;
		if(sim->trial_converge != 0){
			trial_converge(sim->trial_converge);
			stat_ratio(R_ratio);
		}

		stat_cycles << fixed;
		stat_cycles << setprecision(0) << cycle               << "\t";
		stat_cycles << setprecision(6) << stat_ratio.mean()   << "\t";
		stat_cycles << setprecision(6) << stat_ratio.stddev() << "\t";
		stat_cycles << setprecision(6) << trial_converge.mean() << "\t";
		stat_cycles << setprecision(6) << trial_converge.stddev() << "\t";
		stat_cycles << setprecision(6) << 1.0*trial_converge.count()/(1.*cycle) << "\n";

		printf("|| Nearest = %2.2f (%2.2f,%2.2f)\tLearned = %2.2f (%2.2f,%2.2f) ->\t(Rmin/Rlearn) = %1.4f learned after %u trials\n",
				sim->e()->nearest()->d(),
				sim->e()->nearest()->x(),
				sim->e()->nearest()->y(),
				sim->c(0)->GV(0).len(),
				sim->c(0)->GV(0).x,
				sim->c(0)->GV(0).y,
				R_ratio,
				sim->trial_converge);
		printf("Cycle completed.\n");
		printf("==========================================================================================================\n");
		printf("Mean ratio = %1.4f +- %1.4f, mean trials for convergence = %2.4f +- %2.4f, convergence rate = %1.4f\n",
				stat_ratio.mean(),
				stat_ratio.stddev(),
				trial_converge.mean(),
				trial_converge.stddev(),
				1.0*trial_converge.count()/(1.*cycle)
				);
		printf("==========================================================================================================\n\n");
		delete sim;
	}

	// Write results into file
	multi_cycletrials << fixed;
	for(unsigned int trial = 0; trial < numtrials; trial++){
		multi_cycletrials << setprecision(0) << trial+1 << "\t";
		multi_cycletrials << setprecision(6) << expl_rate.at(trial).mean()   << "\t";
		multi_cycletrials << setprecision(6) << expl_rate.at(trial).stddev() << "\t";
		multi_cycletrials << setprecision(6) << home_rate.at(trial).mean()   << "\t";
		multi_cycletrials << setprecision(6) << home_rate.at(trial).stddev() << "\t";
		multi_cycletrials << setprecision(6) << goal_rate.at(trial).mean()   << "\t";
		multi_cycletrials << setprecision(6) << goal_rate.at(trial).stddev() << "\n";
	}
	multi_cycletrials.close();
	stat_cycles.close();




	auto elapsed_secs_cl = timer.Elapsed();
	printf("%4.3f s. Done.\n", elapsed_secs_cl.count()/1000.);
}


