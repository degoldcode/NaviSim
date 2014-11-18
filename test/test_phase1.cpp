/*
 * test_phase1.cpp
 *
 *  Created on: Nov 18, 2014
 *      Author: degoldschmidt
 */

#include "../src/environment.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
using namespace std;

Environment* env;
const int numagents= 1;
const double T= 1000.;
const double dt= 0.1;
int tstep= 0;
ofstream agent_str;

double randn(double mean, double var){
	static random_device e{};
	static normal_distribution<double> d(mean, var);
	return d(e);
}

int main(){
	agent_str.open("data/agent.dat");
	env = new Environment(numagents);
	while(tstep*dt< T){
		agent_str << tstep*dt;
		for(unsigned int i= 0; i< numagents; i++){
			agent_str << "\t" << env->a(i)->x()<< "\t" << env->a(i)->y();
			env->update(randn(0.,1.));
		}
		agent_str << endl;
		tstep++;
	}
}
