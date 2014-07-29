/*
 * environment.cpp
 *
 *  Created on: Jul 17, 2014
 *      Author: degoldschmidt
 */

#include <cmath>
#include "environment.h"
using namespace std;

Environment::Environment(double num_agents, double num_goals, double num_landmarks, double max_radius){
	stream_g.open("./data/goals.dat");
	stream_lm.open("./data/landmarks.dat");
	for(unsigned int i = 0; i < num_agents; i++){
		Agent * const agent = new Agent();
		agent_list.push_back(agent);
	}
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
				if(get_distance(goal_list.at(j), goal) > 5.){
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
				if(get_distance(landmark, goal_list.at(j)) > 2.){
					count++;
				}
			}
			for(unsigned int j = 0; j < landmark_list.size(); j++){
				//cout << j << " " << get_distance(landmark_list.at(j), landmark) << endl;
				if(get_distance(landmark_list.at(j), landmark) > 3.){
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
	reward = 0.0;
	lm_recogn.resize(num_landmarks);
}

Environment::~Environment(){
	for(unsigned int i = 0; i < agent_list.size(); i++)
		delete agent_list.at(i);
	for(unsigned int i = 0; i < goal_list.size(); i++){
		stream_g << goal_list.at(i)->x_position << "\t" << goal_list.at(i)->y_position << "\t" << goal_list.at(i)->thit << endl;
		delete goal_list.at(i);
	}
	stream_g.close();
	for(unsigned int i = 0; i < landmark_list.size(); i++){
		stream_lm << landmark_list.at(i)->x_position << "\t" << landmark_list.at(i)->y_position << "\t" << landmark_list.at(i)->hit << endl;
		delete landmark_list.at(i);
	}
	stream_lm.close();
}

void Environment::update(double command){
	reward = 0.0;
	for(unsigned int i = 0; i < agent_list.size(); i++){
		agent_list.at(i)->update(command);
		for(unsigned int j = 0; j < goal_list.size(); j++)
			reward += goal_list.at(j)->get_reward(agent_list.at(i)->x, agent_list.at(i)->y);
		for(unsigned int j = 0; j < landmark_list.size(); j++)
			lm_recogn.at(j) = landmark_list.at(j)->get_hit(agent_list.at(i)->x, agent_list.at(i)->y);
	}
}

void Environment::add_goal(double x, double y){
	Goal* goal = new Goal(x,y);
	goal_list.push_back(goal);
}

int Environment::get_hits(){
	int sum = 0;
	for(unsigned int j = 0; j < goal_list.size(); j++)
		sum += goal_list.at(j)->hit;
	return sum;
}

void Environment::reset(){
	for(unsigned int i = 0; i < agent_list.size(); i++)
		agent_list.at(i)->reset();
	for(unsigned int j = 0; j < goal_list.size(); j++)
		goal_list.at(j)->hit = 0;
}

double Environment::get_distance(Goal* goal1, Goal* goal2){
	return sqrt(pow(goal1->x_position-goal2->x_position,2.) + pow(goal1->y_position-goal2->y_position,2.));
}
double Environment::get_distance(Landmark* landmark1, Goal* goal2){
	return sqrt(pow(landmark1->x_position-goal2->x_position,2.) + pow(landmark1->y_position-goal2->y_position,2.));
}
double Environment::get_distance(Landmark* landmark1, Landmark* landmark2){
	return sqrt(pow(landmark1->x_position-landmark2->x_position,2.) + pow(landmark1->y_position-landmark2->y_position,2.));
}

