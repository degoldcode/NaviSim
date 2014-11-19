/*****************************************************************************
 *  environment.cpp                                                          *
 *                                                                           *
 *  Created on:   Jul 17, 2014                                               *
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

#include <cmath>
#include "environment.h"
using namespace std;

Environment::Environment(int num_agents){
	inv_sampling_rate = 1;
	stream_h.open("./data/home.dat");

	/*** SET UP AGENTS ***/
	for(unsigned int i = 0; i < num_agents; i++){
		Agent * const agent = new Agent(inv_sampling_rate);
		agent_list.push_back(agent);
	}
}

Environment::Environment(int num_goals, int num_landmarks, double max_radius, int num_agents){
	stream_h.open("./data/home.dat");
/*	stream_g.open("./data/goals.dat");
	stream_lm.open("./data/landmarks.dat");
	stream_p.open("./data/pipes.dat");
	stream_food.open("./data/food.dat");*/

	/*** SET UP AGENTS ***/
	for(unsigned int i = 0; i < num_agents; i++){
		Agent * const agent = new Agent(inv_sampling_rate);
		agent_list.push_back(agent);
	}

/*	** SET UP GOALS **
	for(unsigned int i = 0; i < num_goals; i++){
		add_goal(max_radius);
	}

	** SET UP LANDMARKS **
	for(unsigned int i = 0; i < num_landmarks; i++){
		add_landmark(max_radius);
	}*/
}

Environment::~Environment(){
	stream_h << "0.0" << "\t" << "0.0" << endl;
	stream_h.close();
	for(unsigned int i = 0; i < agent_list.size(); i++)
		delete agent_list.at(i);
	/*for(unsigned int i = 0; i < goal_list.size(); i++){
		stream_g << goal_list.at(i)->x() << "\t"
				 << goal_list.at(i)->y() << "\t"
				 << goal_list.at(i)->total() << "\t"
				 << goal_list.at(i)->color()
				 << endl;
		delete goal_list.at(i);
	}
	stream_g.close();
	for(unsigned int i = 0; i < landmark_list.size(); i++){
		stream_lm << landmark_list.at(i)->x() << "\t"
				  << landmark_list.at(i)->y() << "\t"
				  << landmark_list.at(i)->total()
				  << endl;
		delete landmark_list.at(i);
	}
	stream_lm.close();
	for(unsigned int i = 0; i < pipe_list.size(); i++){
		stream_p << pipe_list.at(i)->x0() << "\t"
				 << pipe_list.at(i)->y0() << "\t"
				 << pipe_list.at(i)->w() << "\n"
				 << pipe_list.at(i)->x1() << "\t"
				 << pipe_list.at(i)->y1()
				 << endl;
		delete pipe_list.at(i);
	}
	stream_p.close();
	stream_food.close();*/
}

/*void Environment::add_goal(double x, double y, int color){
	Goal* goal = new Goal(x,y,color);
	goal_list.push_back(goal);
}*/

/*void Environment::add_goal(double max_radius){
	flag = false;
	Goal* goal;
	while(!flag){
		goal = new Goal(max_radius);
		if(goal_list.size()==0)
			flag = true;
		count = 0;
		for(unsigned int j = 0; j < goal_list.size(); j++){
			if(d(goal_list.at(j), goal) > 3.){
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
}*/

/*void Environment::add_landmark(double x, double y){
	Landmark* lm = new Landmark(x,y);
	landmark_list.push_back(lm);
}*/

/*void Environment::add_landmark(double max_radius){
	Landmark* landmark;
	flag = false;
	while(!flag){
		landmark = new Landmark(max_radius);
		if(landmark_list.size()==0)
			flag = true;
		count = 0;
		for(unsigned int j = 0; j < goal_list.size(); j++){
			if(d(landmark, goal_list.at(j)) > .5){
				count++;
			}
		}
		for(unsigned int j = 0; j < landmark_list.size(); j++){
			if(d(landmark_list.at(j), landmark) > 1.){
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
}*/

/*void Environment::add_pipe(double x0, double x1, double y0, double y1){
	Pipe* pipe = new Pipe(x0,x1,y0,y1);
	pipe_list.push_back(pipe);
}*/

Agent* Environment::a(int i){
	return agent_list.at(i);
}

/*int Environment::color(){
	if(goal_list.size()>0){
		int out = nearest(x(),y())->color();
		agent_list.at(0)->set_type(out);
		return out;
	}
	else
		return 0;
}*/

/*double Environment::d(Goal* g1, Goal* g2){
	return sqrt(d(g1->x(),g2->x()) + d(g1->y(),g2->y()));
}

double Environment::d(Landmark* lm1, Goal* g2){
	return sqrt(d(lm1->x(),g2->x()) + d(lm1->y(),g2->y()));
}

double Environment::d(Landmark* lm1, Landmark* lm2){
	return sqrt(d(lm1->x(),lm2->x()) + d(lm1->y(),lm2->y()));
}

double Environment::d(double x0, double x1){
	return pow(x0-x1,2);
}*/

/*int Environment::get_hits(){
	int sum = 0;
	for(unsigned int j = 0; j < goal_list.size(); j++)
		sum += goal_list.at(j)->hits();
	return sum;
}*/

/*int Environment::get_hits(int i){
	int sum = 0;
	for(unsigned int j = 0; j < goal_list.size(); j++){
		if(goal_list.at(j)->color()==i)
			sum += goal_list.at(j)->hits();
	}
	return sum;
}*/

/*double Environment::get_sum_reward(){
	return sum_reward;
}*/

/*double Environment::get_total_reward(){
	return total_reward;
}*/

/*Goal* Environment::goal(int i){
	return goal_list.at(i);
}*/

/*double Environment::lmr(){
	return lm_recogn;
}*/

/*int Environment::n_goals(){
	return goal_list.size();
}*/

/*Goal* Environment::nearest(double x, double y){
	double min_dist;
	if(goal_list.size()>0)
		min_dist = sqrt( d(goal_list.at(0)->x(), x) + d(goal_list.at(0)->y(), y));
	else
		return 0;
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
}*/

/*double Environment::r(){
	return reward;
}*/

void Environment::reset(){
	sum_reward = 0.0;
	for(unsigned int i = 0; i < agent_list.size(); i++)
		agent_list.at(i)->reset();
/*	for(unsigned int j = 0; j < goal_list.size(); j++)
		goal_list.at(j)->reset();*/
}

void Environment::set_mode(int in_mode){
	mode = in_mode;
}

/*void Environment::swap_reward(){
	for(int i=0; i<goal_list.size(); i++)
		goal_list.at(i)->swap();
}*/

double Environment::th(int i){
	return agent_list.at(i)->th();
}

void Environment::update(){
	reward = 0.0;
	lm_recogn = 0.0;

	update_agents();

//		for(unsigned int j = 0; j < goal_list.size(); j++)
//			reward += goal_list.at(j)->r(agent_list.at(i)->x(), agent_list.at(i)->y(), mode);
//		for(unsigned int j = 0; j < landmark_list.size(); j++)
//			lm_recogn += landmark_list.at(j)->pod(agent_list.at(i)->x(), agent_list.at(i)->y());
	sum_reward += reward;
	total_reward += reward;
}

void Environment::update_agents(){
	for(unsigned int i = 0; i < agent_list.size(); i++){
/*		for(unsigned int j = 0; j < pipe_list.size(); j++)
			agent_list.at(i) = pipe_list.at(j)->set_agent_pipe(agent_list.at(i));*/
		agent_list.at(i)->update();
	}
}

double Environment::x(int i){
	return agent_list.at(i)->x();
}

double Environment::y(int i){
	return agent_list.at(i)->y();
}
