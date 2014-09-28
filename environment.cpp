/*
 * environment.cpp
 *
 *  Created on: Jul 17, 2014
 *      Author: degoldschmidt
 */

#include <cmath>
#include "environment.h"
using namespace std;

Environment::Environment(double num_agents){
	stream_g.open("./data/goals.dat");
	stream_lm.open("./data/landmarks.dat");
	stream_p.open("./data/pipes.dat");
	stream_h.open("./data/home.dat");
	inv_sampling_rate = 1;

	/*** SET UP AGENTS ***/
	for(unsigned int i = 0; i < num_agents; i++){
		Agent * const agent = new Agent(inv_sampling_rate);
		agent_list.push_back(agent);
	}

	/*** SET UP CLASS VARIABLES ***/
	reward = 0.0;
	sum_reward = 0.0;
	lm_recogn = 0.0;
	count = 0;
	flag = 0;
	mode = 0;
}

Environment::Environment(double num_agents, double num_goals, double num_landmarks, double max_radius){
	stream_g.open("./data/goals.dat");
	stream_lm.open("./data/landmarks.dat");
	stream_p.open("./data/pipes.dat");
	stream_h.open("./data/home.dat");
	stream_food.open("./data/food.dat");

	/*** SET UP AGENTS ***/
	for(unsigned int i = 0; i < num_agents; i++){
		Agent * const agent = new Agent(inv_sampling_rate);
		agent_list.push_back(agent);
	}

	/*** SET UP GOALS AND LANDMARKS ***/
	for(unsigned int i = 0; i < num_goals; i++){
		flag = false;
		Goal* goal;
		while(!flag){
			goal = new Goal(max_radius);
			if(goal_list.size()==0)
				flag = true;
			count = 0;
			for(unsigned int j = 0; j < goal_list.size(); j++){
				//cout << j << " " << get_distance(goal_list.at(j), goal) << endl;
				if(get_distance(goal_list.at(j), goal) > 3.){
					count++;
				}
			}
			if(count==goal_list.size())
				flag = true;
			if(!flag){
				//cout << "Warning: Goal too close to each other.\n";
				delete goal;
			}
		}
		goal_list.push_back(goal);
	}


	for(unsigned int i = 0; i < num_landmarks; i++){
		Landmark* landmark;
		flag = false;
		while(!flag){
			landmark = new Landmark(max_radius);
			if(landmark_list.size()==0)
				flag = true;
			count = 0;
			for(unsigned int j = 0; j < goal_list.size(); j++){
				//cout << j << " " << get_distance(goal_list.at(j), landmark) << endl;
				if(get_distance(landmark, goal_list.at(j)) > .5){
					count++;
				}
			}
			for(unsigned int j = 0; j < landmark_list.size(); j++){
				//cout << j << " " << get_distance(landmark_list.at(j), landmark) << endl;
				if(get_distance(landmark_list.at(j), landmark) > 1.){
					count++;
				}
			}
			if(count==goal_list.size()+landmark_list.size())
				flag = true;
			if(!flag){
				//cout << "Warning: Landmark too close to each other.\n";
				delete landmark;
			}
		}
		landmark_list.push_back(landmark);
	}

	/*** SET UP PIPES ***/
	//// HERE


	/*** SET UP CLASS VARIABLES ***/
	reward = 0.0;
	sum_reward = 0.0;
	mode = 0;
	count = 0;
	flag = 0;
	lm_recogn = 0.0;
}

Environment::~Environment(){
	stream_h << "0.0" << "\t" << "0.0" << endl;
	stream_h.close();
	for(unsigned int i = 0; i < agent_list.size(); i++)
		delete agent_list.at(i);
	for(unsigned int i = 0; i < goal_list.size(); i++){
		stream_g << goal_list.at(i)->x() << "\t" << goal_list.at(i)->y() << "\t" << goal_list.at(i)->total_hits << "\t" << goal_list.at(i)->type() << endl;
		delete goal_list.at(i);
	}
	stream_g.close();
	for(unsigned int i = 0; i < landmark_list.size(); i++){
		stream_lm << landmark_list.at(i)->x() << "\t" << landmark_list.at(i)->y() << "\t" << landmark_list.at(i)->total_hits << endl;
		delete landmark_list.at(i);
	}
	stream_lm.close();
	for(unsigned int i = 0; i < pipe_list.size(); i++){
		stream_p << pipe_list.at(i)->x_pos_0 << "\t" << pipe_list.at(i)->y_pos_0 << "\t" << pipe_list.at(i)->p_width << "\n" << pipe_list.at(i)->x_pos_1 << "\t" << pipe_list.at(i)->y_pos_1 << endl;
		delete pipe_list.at(i);
	}
	stream_p.close();
	stream_food.close();
}

void Environment::update(double command){
	reward = 0.0;
	lm_recogn = 0.0;
	stream_food << agent_list.at(0)->t << " " ;
	for(unsigned int j = 0; j < goal_list.size(); j++)
			stream_food <<  goal_list.at(j)->a() << " ";
	stream_food << endl;
	for(unsigned int i = 0; i < agent_list.size(); i++){
		for(unsigned int j = 0; j < pipe_list.size(); j++)
			agent_list.at(i) = pipe_list.at(j)->set_agent_pipe(agent_list.at(i));
		for(unsigned int j = 0; j < pipe_list.size(); j++)
			if(pipe_list.at(j)->in_this_pipe)
				agent_list.at(i)->in_pipe = true;
		agent_list.at(i)->update(command);
		for(unsigned int j = 0; j < goal_list.size(); j++)
			reward += goal_list.at(j)->r(agent_list.at(i)->get_x(), agent_list.at(i)->get_y(), mode);
		for(unsigned int j = 0; j < landmark_list.size(); j++)
			lm_recogn += landmark_list.at(j)->get_hit(agent_list.at(i)->get_x(), agent_list.at(i)->get_y());
	}
	sum_reward += reward;
}

void Environment::add_goal(double x, double y, int color){
	Goal* goal = new Goal(x,y,color);
	goal_list.push_back(goal);
}

void Environment::add_landmark(double x, double y){
	Landmark* lm = new Landmark(x,y);
	landmark_list.push_back(lm);
}

void Environment::add_pipe(double x0, double x1, double y0, double y1, double width){
	Pipe* pipe = new Pipe(x0,x1,y0,y1,width);
	pipe_list.push_back(pipe);
}

int Environment::get_hits(){
	int sum = 0;
	for(unsigned int j = 0; j < goal_list.size(); j++)
		sum += goal_list.at(j)->hit;
	return sum;
}

int Environment::get_hits(int i){
	int sum = 0;
	for(unsigned int j = 0; j < goal_list.size(); j++){
		if(goal_list.at(j)->type()==i)
			sum += goal_list.at(j)->hit;
	}
	return sum;
}

void Environment::reset(){
	sum_reward = 0.0;
	for(unsigned int i = 0; i < agent_list.size(); i++)
		agent_list.at(i)->reset();
	for(unsigned int j = 0; j < goal_list.size(); j++)
		goal_list.at(j)->hit = 0;
}

int Environment::type(){
	int type = nearest(getx(),gety())->type();
	agent_list.at(0)->set_type(type);
	return type;
}

double Environment::get_distance(Goal* g1, Goal* g2){
	return sqrt(d(g1->x(),g2->x()) + d(g1->y(),g2->y()));
}
double Environment::get_distance(Landmark* lm1, Goal* g2){
	return sqrt(d(lm1->x(),g2->x()) + d(lm1->y(),g2->y()));
}
double Environment::get_distance(Landmark* lm1, Landmark* lm2){
	return sqrt(d(lm1->x(),lm2->x()) + d(lm1->y(),lm2->y()));
}

double Environment::getx(){
	return agent_list.at(0)->get_x();
}

double Environment::gety(){
	return agent_list.at(0)->get_y();
}

double Environment::get_real_HV(){
	return agent_list.at(0)->theta;
}

Goal* Environment::nearest(double x, double y){
	double min_dist = sqrt( d(goal_list.at(0)->x(), x) + d(goal_list.at(0)->y(), y));
	double dist;
	int idx=0;
	for(int i=1; i<goal_list.size(); i++){
		dist = sqrt( d(goal_list.at(i)->x(), x) + d(goal_list.at(i)->y(), y));
		if(dist<min_dist){
			idx = i;
			min_dist = dist;
		}
	}
	return goal_list.at(idx);
}

double Environment::d(double x0, double x1){
	return pow(x0-x1,2);
}

void Environment::swap_reward(){
	for(int i=0; i<goal_list.size(); i++)
		goal_list.at(i)->swap();
}


