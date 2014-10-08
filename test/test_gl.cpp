/*
 * test_gl.cpp
 *
 *  Created on: Oct 8, 2014
 *      Author: degoldschmidt
 */

#include <armadillo>
#include "../environment.h"
#include "../pin.h"
#include "../goallearning.h"
using namespace arma;

Environment* ENV;
Agent* my_agent;
PIN* PI;
GoalLearning* GVL;
const int total_time = 5000;
const int trials = 1;
ofstream traj;
mat hws;

int main(){
	traj.open("./test/traj.dat");

	ENV = new Environment();
	my_agent = ENV->agent(0);
	my_agent->set_phi(M_PI/2.);
	ENV->add_goal(0.0,10.0);

	PI = new PIN(360, 0.0, 0.0, 0.0);
	GVL = new GoalLearning(360, 0.0, false);


	for(int trial = 0; trial < trials; trial++){
		for(int i = 0; i < total_time; i++){
			if(my_agent->y() > 10.0){
				my_agent->to(0.,0.);
				PI->reset();
			}
			hws = join_rows(hws, GVL->rate());
			traj << PI->x() << " "
			     << PI->y() << " "
			     << my_agent->x() << " "
			     << my_agent->y() << " "
			     << GVL->x() << " "
			     << GVL->y() << endl;
			//ENV->update(1.*randnum());
			ENV->update(0.0);
			PI->update(my_agent->phi(), my_agent->v());
			vec PI_act = PI->rate();
			double reward = ENV->r();
			GVL->update(PI_act, reward, 1.0);

			if(i%100==0){
				printf("PI = %e\t", PI->len());
				printf("Real = %e\n", my_agent->d());
				//printf("PI (x,y) = (%g,%g)\t", PI->x(), PI->y());
				//printf("Real (x,y) = (%g,%g)\n", my_agent->x(), my_agent->y());
			}

		}
	}

	hws.save("./test/weights.mat", raw_ascii);
	traj.close();
	delete GVL;
	delete PI;
	delete ENV;
}

