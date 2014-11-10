/*
 * main.cpp
 *
 *  Created on: 01.08.2014
 *      Author: meicron
 */

#include <armadillo>
#include <fstream>
#include <string>
#include "simulation.h"
#include "timer.h"
using namespace std;
using namespace arma;

const bool par_scan = false;
const int max_idx = 6;
double param_array[max_idx]={0.0,0.01,0.02,0.05,0.1,0.2};

const int num_trials = 10;
const int max_number_exp = 1;
string param_type = "sensory_noise";

ofstream write_gl;

Simulation* my_sim;

int main(){
	Timer timer(true);
	my_sim = new Simulation(param_type, num_trials);
	double param;
	write_gl.open("./data/allcycles.dat");

	if(par_scan)
		printf("\nStart simulation with varying %s from %g to %g. Averaged over %u trials.\n\n", param_type.c_str(), param_array[0], param_array[max_idx-1], num_trials);
	else
		printf("\nStart simulation for %u cycles with %u trials.\n\n", max_number_exp, num_trials);
	int idx = 0;
	while(idx < max_idx){
		my_sim->sens_noise = 0.00;
		running_stat<double> conv_time;
		running_stat<double> succ_rate;
		int flags = 0;
		for(int learn_exp_idx = 0; learn_exp_idx < max_number_exp; learn_exp_idx++){
			//param = param_array[idx];
			printf("\nStart learning cycle %u\n\n", learn_exp_idx+1);
			my_sim->run();
			if(my_sim->get_lrun()< num_trials){
				conv_time(double(my_sim->get_lrun()));
				succ_rate(double(my_sim->get_srate()));
			}
			else
				flags++;
			if(learn_exp_idx%10==0){
				printf("\n=== Avg Total reward = %5.3f +- %5.3f ===\n", my_sim->total_reward.mean(), my_sim->total_reward.stddev());
				printf("\n=== Avg convergence at %3.1f +- %3.1f\tflags = %u ===\n", conv_time.mean(),conv_time.stddev(),flags);
				printf("=== Avg SUCCESS RATE at %3.3f +- %3.3f,\tMIN = %3.3f,\tMAX = %3.3f ===\n\n", succ_rate.mean(),succ_rate.stddev(),succ_rate.min(),succ_rate.max());
			}
		}
		printf("\n===+++ END Avg Total reward = %5.3f +- %5.3f +++===\n", my_sim->total_reward.mean(), my_sim->total_reward.stddev());
		printf("\n===+++ END Avg convergence at %3.1f +- %3.1f,\tMIN = %3.1f,\tMAX = %3.1f,\tflags = %u END +++===\n", conv_time.mean(),conv_time.stddev(),conv_time.min(),conv_time.max(),flags);
		printf("===+++ END Avg SUCCESS RATE at %3.3f +- %3.3f,\tMIN = %3.3f,\tMAX = %3.3f END +++===\n\n", succ_rate.mean(),succ_rate.stddev(),succ_rate.min(),succ_rate.max());
		idx++;
		if(!par_scan)
			idx = max_idx;
	}
	for(int trial = 0; trial < num_trials; trial++){
		write_gl << my_sim->explor_rate_avg.at(trial).mean()
				<< "\t" << (1.96/sqrt(max_number_exp))*my_sim->explor_rate_avg.at(trial).stddev()
				<< "\t" << my_sim->success_rate_avg.at(trial).mean()
				<< "\t" << (1.96/sqrt(max_number_exp))*my_sim->success_rate_avg.at(trial).stddev()
				<< "\t" << my_sim->explor_rate_avg2.at(trial).mean()
				<< "\t" << (1.96/sqrt(max_number_exp))*my_sim->explor_rate_avg2.at(trial).stddev()
				<< "\t" << my_sim->success_rate_avg2.at(trial).mean()
				<< "\t" << (1.96/sqrt(max_number_exp))*my_sim->success_rate_avg2.at(trial).stddev()
				<< "\t" << my_sim->prob_B.at(trial).mean()
				<< "\t" << (1.96/sqrt(max_number_exp))*my_sim->prob_B.at(trial).stddev()
				<< "\t" << my_sim->prob_Y.at(trial).mean()
				<< "\t" << (1.96/sqrt(max_number_exp))*my_sim->prob_Y.at(trial).stddev()
				<< "\t" << my_sim->choice.at(trial).mean()
				<< "\t" << (1.96/sqrt(max_number_exp))*my_sim->choice.at(trial).stddev()
				<< endl;
	}

	delete my_sim;
	write_gl.close();
	auto elapsed_secs_cl = timer.Elapsed();
	printf("%4.3f s. Done.\n", elapsed_secs_cl.count()/1000.);
}


