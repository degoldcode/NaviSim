/*
 * test_gl.cpp
 *
 *  Created on: Oct 8, 2014
 *      Author: degoldschmidt
 */

#include "../environment.h"
#include "../pin.h"
#include "../goallearning.h"

Environment* ENV;
Agent* my_agent;
PIN* PI;
GoalLearning* GVL;
const int total_time = 50;
const int trials = 1;
ofstream traj;

int main(){
	traj.open("./test/traj.dat");

	ENV = new Environment();
	my_agent = ENV->agent(0);
	ENV->add_pipe(0.0,0.0,0.0,5.0);
	ENV->add_goal(0.0,5.0);

	PI = new PIN(360, 0.0, 0.0, 0.0);
	GVL = new GoalLearning(360, 0.0, false);


	for(int trial = 0; trial < trials; trial++){
		for(int i = 0; i < total_time; i++){
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
			//printf("PI = %e\t", my_pin->len());
			//printf("Real = %e\n", my_agent->d());
			//printf("PI (x,y) = (%g,%g)\t", my_pin->x(), my_pin->y());
			//printf("Real (x,y) = (%g,%g)\n", my_agent->x(), my_agent->y());

		}
	}

	traj.close();
	delete GVL;
	delete PI;
	delete ENV;
}

