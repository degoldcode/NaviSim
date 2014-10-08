/*
 * test_pi.cpp
 *
 *  Created on: Oct 8, 2014
 *      Author: degoldschmidt
 */

#include "../agent.h"
#include "../pin.h"
#include <fstream>

Agent* my_agent;
PIN* my_pin;
const int total_time = 1000;
const int trials = 1;
running_stat<double> error;
ofstream traj;

double randnum(){
	static random_device e{};
	static normal_distribution<double> d(0., 1.);
	return d(e);
};

int main(){
	traj.open("./test/traj.dat");
	my_agent = new Agent(1);
	my_agent->no_write();
	my_pin = new PIN(360, 0.0, .00,.0);

	for(int trial = 0; trial < trials; trial++){
		for(int i = 0; i < total_time; i++){
			traj << my_pin->x() << " "
			     << my_pin->y() << " "
			     << my_agent->x() << " "
			     << my_agent->y() << endl;
			//my_agent->update(0.2);
			//my_agent->update(1.*randnum());
			my_pin->update(my_agent->phi(), my_agent->v());
			//printf("PI = %e\t", my_pin->len());
			//printf("Real = %e\n", my_agent->d());
			//printf("PI (x,y) = (%g,%g)\t", my_pin->x(), my_pin->y());
			//printf("Real (x,y) = (%g,%g)\n", my_agent->x(), my_agent->y());

		}
		error(180.*abs(my_agent->th()-my_pin->avg())/M_PI);
		//printf("Error = %g°\n", 180.*abs(my_agent->th()-my_pin->avg())/M_PI);
	}
	printf("Avg Error = (%g+-%g)°\n", error.mean(), error.stddev());

	traj.close();
	delete my_pin;
};


