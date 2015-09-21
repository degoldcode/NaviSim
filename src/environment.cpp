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
	stop_trial = false;


	(VERBOSE)?printf("\nCREATE %u AGENTS\n\n", num_agents):VERBOSE;
	/*** SET UP AGENTS ***/
	for(unsigned int i = 0; i < num_agents; i++){
		Agent * const agent = new Agent(VERBOSE);
		agent_list.push_back(agent);
	}

	reward.resize(agent_list.size());
	trial_reward.resize(agent_list.size());
	total_reward.resize(agent_list.size());
	lm_recogn.resize(agent_list.size());
	g_stats.collisions = zeros<mat>(agent_list.size(), goal_list.size());
	g_stats.hits = zeros<mat>(agent_list.size(), goal_list.size());
	lm_stats.visible = zeros<mat>(landmark_list.size(), agent_list.size());
	in_pipe = zeros<mat>(agent_list.size(), pipe_list.size());
	open_streams();
}

Environment::Environment(int num_goals, int num_landmarks, double max_radius, int num_agents){
	inv_sampling_rate = 1;

	(VERBOSE)?printf("\nCREATE %u AGENTS\n", num_agents):VERBOSE;

	/*** SET UP AGENTS ***/
	for(unsigned int i = 0; i < num_agents; i++){
		Agent * const agent = new Agent(VERBOSE);
		agent_list.push_back(agent);
	}
	(VERBOSE)?printf("\nAGENTS CREATED\n\n"):VERBOSE;
	(VERBOSE)?printf("\nCREATE %u GOALS\n\n", num_goals):VERBOSE;
	/** SET UP GOALS **/
	for(unsigned int i = 0; i < num_goals; i++){
		add_goal(max_radius);
	}
	(VERBOSE)?printf("\nGOALS CREATED\n\n"):VERBOSE;
	(VERBOSE)?printf("\nCREATE %u LANDMARKS\n\n", num_landmarks):VERBOSE;
	/** SET UP LANDMARKS **/
	for(unsigned int i = 0; i < num_landmarks; i++){
		add_landmark(max_radius);
	}
	(VERBOSE)?printf("\nLANDMARKS CREATED\n\n"):VERBOSE;

	reward.resize(agent_list.size());
	trial_reward.resize(agent_list.size());
	total_reward.resize(agent_list.size());
	lm_recogn.resize(agent_list.size());
	g_stats.collisions = zeros<mat>(agent_list.size(), goal_list.size());
	g_stats.hits = zeros<mat>(agent_list.size(), goal_list.size());
	lm_stats.visible = zeros<mat>(landmark_list.size(), agent_list.size());
	lm_stats.seen = zeros<mat>(landmark_list.size(), agent_list.size());
	lm_stats.catchment = zeros<mat>(landmark_list.size(), agent_list.size());
	open_streams();
}

Environment::~Environment(){
	stream_h << "0.0" << "\t" << "0.0" << endl;
	stream_h.close();
	for(unsigned int i = 0; i < agent_list.size(); i++)
		delete agent_list.at(i);
	for(unsigned int i = 0; i < goal_list.size(); i++){
		stream_g << goal_list.at(i)->x() << "\t"
				 << goal_list.at(i)->y() << "\t"
				 << g_stats.hits(0, i) << "\t"
				 << goal_list.at(i)->color()
				 << endl;
		delete goal_list.at(i);
	}
	stream_g.close();
	for(unsigned int i = 0; i < landmark_list.size(); i++){
		stream_lm << landmark_list.at(i)->x() << "\t"
				  << landmark_list.at(i)->y()
				  << endl;
		delete landmark_list.at(i);
	}
	stream_lm.close();
//	for(unsigned int i = 0; i < pipe_list.size(); i++){
//		stream_p << pipe_list.at(i)->x0() << "\t"
//				 << pipe_list.at(i)->y0() << "\t"
//				 << pipe_list.at(i)->w() << "\n"
//				 << pipe_list.at(i)->x1() << "\t"
//				 << pipe_list.at(i)->y1()
//				 << endl;
//		delete pipe_list.at(i);
//	}
	stream_p.close();
	stream_food.close();
}

Agent* Environment::a(int i){
	return agent_list.at(i);
}

void Environment::a(double x, double y){
	add_agent(x,y);
}

void Environment::add_agent(double x, double y){
	Agent * const agent = new Agent(x,y);
	agent_list.push_back(agent);
	g_stats.collisions = zeros<mat>(agent_list.size(), goal_list.size());
	g_stats.hits = zeros<mat>(agent_list.size(), goal_list.size());
}

void Environment::add_goal(double x, double y, int color, double size, bool decay){
	Goal* goal = new Goal(x,y,VERBOSE,color, size, decay);
	goal_list.push_back(goal);
	g_stats.collisions = zeros<mat>(agent_list.size(), goal_list.size());
	g_stats.hits = zeros<mat>(agent_list.size(), goal_list.size());
}

void Environment::add_goal(double max_radius){
	flag = false;
	Goal* goal;
	while(!flag){
		goal = new Goal(max_radius,VERBOSE);
		if(goal_list.size()==0)
			flag = true;
		count = 0;
		for(unsigned int j = 0; j < goal_list.size(); j++){
			if(d(goal_list.at(j), goal) > 3.){
				count++;
			}
			else
			{
				(VERBOSE)?printf("Too close = %g\n", d(goal_list.at(j), goal)):VERBOSE;
			}
		}
		if(count==goal_list.size())
			flag = true;
		if(!flag){
			(VERBOSE)?printf("\nWARNING: Goal too close to each other.\nGOAL DELETED\n\n"):VERBOSE;
			delete goal;
		}
	}
	goal_list.push_back(goal);
	g_stats.collisions = zeros<mat>(agent_list.size(), goal_list.size());
	g_stats.hits = zeros<mat>(agent_list.size(), goal_list.size());
}

void Environment::add_landmark(double x, double y){
	Landmark* lm = new Landmark(x,y,VERBOSE);
	landmark_list.push_back(lm);
	lm_stats.visible = zeros<mat>(landmark_list.size(), agent_list.size());
	lm_stats.seen = zeros<mat>(landmark_list.size(), agent_list.size());
	lm_stats.catchment = zeros<mat>(landmark_list.size(), agent_list.size());
}

void Environment::add_landmark(double max_radius){
	Landmark* landmark;
	flag = false;
	while(!flag){
		landmark = new Landmark(max_radius,VERBOSE);
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
			(VERBOSE)?printf("\nWARNING: Landmark too close to each other.\nLANDMARK DELETED\n\n"):VERBOSE;
			delete landmark;
		}
	}
	landmark_list.push_back(landmark);
}

void Environment::add_pipe(double x0, double x1, double y0, double y1){
	Pipe* pipe = new Pipe(x0,x1,y0,y1);
	pipe_list.push_back(pipe);
	Angle* angle = &(pipe->alpha);
	pipe_angle.push_back(angle);
	in_pipe = zeros<mat>(agent_list.size(), pipe_list.size());
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

double Environment::d(Object* o1, Object* o2){
	return (o1->v() - o2->v()).len();
}

/*double Environment::d(Landmark* lm1, Goal* g2){
	return sqrt(d(lm1->x(),g2->x()) + d(lm1->y(),g2->y()));
}

double Environment::d(Landmark* lm1, Landmark* lm2){
	return sqrt(d(lm1->x(),lm2->x()) + d(lm1->y(),lm2->y()));
}*/

double Environment::d(double x0, double x1){
	return pow(x0-x1,2);
}

/*int Environment::get_hits(){
	int sum = 0;
	for(unsigned int j = 0; j < goal_list.size(); j++)
		sum += goal_list.at(j)->hits();
	return sum;
}*/

int Environment::get_hits(int i){
	int sum = 0;
	for(unsigned int j = 0; j < goal_list.size(); j++){
		sum += g_stats.hits(i, j);
	}
	return sum;
}

double Environment::get_trial_r(int index){
	return trial_reward.at(index);
}

double Environment::get_total_r(int index){
	return total_reward.at(index);
}

Goal* Environment::g(int i){
	return goal_list.at(i);
}

vec Environment::lmr(int i){
	if(i < lm_stats.visible.n_cols)
		return lm_stats.visible.col(i);
	else
		return vec(0.0);
}

/*int Environment::n_goals(){
	return goal_list.size();
}*/

Goal* Environment::nearest(double x, double y){
	double min_dist;
	cout << goal_list.size() << endl;
	if(goal_list.size()>0)
		min_dist = sqrt( d(goal_list.at(0)->x(), x) + d(goal_list.at(0)->y(), y));
	else
		return new Goal(0., 0.);
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

Angle Environment::phi(Object* o1, Object* o2){
	return (o1->v() - o2->v()).ang();
}

void Environment::open_streams(){
//	stream_a.resize(agent_list.size());
//	if(stream_a.size() > 1)
//		for(unsigned int i = 0; i < agent_list.size(); i++)
//			stream_a.at(i).open("TODO");
//	else
//		stream_a.at(0).open("agent.dat");
	stream_h.open("./data/home.dat");
	stream_g.open("./data/goals.dat");
	stream_lm.open("./data/landmarks.dat");
	stream_p.open("./data/pipes.dat");
	stream_food.open("./data/food.dat");
}

/*double Environment::r(){
	return reward;
}*/

void Environment::reset(){
	stop_trial = false;
	g_stats.collisions = zeros<mat>(agent_list.size(), goal_list.size());
	g_stats.hits = zeros<mat>(agent_list.size(), goal_list.size());
	std::fill(trial_reward.begin(), trial_reward.end(), 0.);
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

Angle Environment::th(int i){
	return agent_list.at(i)->th();
}

void Environment::update(){
	update_rewards();
	update_collisions();
	update_pipe();
	update_agents();
}

void Environment::update_agents(){
	for(unsigned int i = 0; i < agent_list.size(); i++){
		for(unsigned int j = 0; j < landmark_list.size(); j++){
			if(lm_stats.catchment(j,i) == 1 && lm_stats.seen(j,i) == 0){
				agent_list.at(i)->lm_catch = true;
				Angle lm_phi = phi(landmark_list.at(i),agent_list.at(i)).deg();
				double landmark_attract = 0.5*(lm_phi - agent_list.at(i)->phi()).S();
				//printf("%g vs. %g -> %g\n", phi(agent_list.at(i), landmark_list.at(i)).deg(), phi(landmark_list.at(i),agent_list.at(i)).deg(), landmark_attract);
				agent_list.at(i)->set_dphi(new Angle(landmark_attract));
			}
			else{
				agent_list.at(i)->set_dphi(new Angle(0.0));
				agent_list.at(i)->lm_catch = false;
			}
		}
		if(i < lm_stats.visible.n_rows)
			agent_list.at(i)->update(reward.at(i), lm_stats.visible.col(i));
		else
			agent_list.at(i)->update(reward.at(i), vec(0.0));
		if((agent_list.at(i)->d() < home_radius && agent_list.at(i)->c()->get_state()) ||
				(!(agent_list.at(i)->c()->homing_on) && agent_list.at(i)->c()->get_state())){
			//printf("stop it\n");
			stop_trial = true;
		}
	}
}

void Environment::update_collisions(){
	for(unsigned int i = 0; i < agent_list.size(); i++){
		for(unsigned int j = 0; j < goal_list.size(); j++){
			if(d(agent_list.at(i), goal_list.at(j)) < goal_radius){
				if(g_stats.collisions(i,j) == 0)
					g_stats.hits(i,j)++;
				g_stats.collisions(i,j) = 1;
			}
			else
				g_stats.collisions(i,j) = 0;
		}
		for(unsigned int j = 0; j < landmark_list.size(); j++){
			if(d(agent_list.at(i), landmark_list.at(j)) < lm_catch_radius){
				lm_stats.catchment(j,i) = 1;
			}
			else{
				lm_stats.catchment(j,i) = 0;
				lm_stats.seen(j,i) = 0;
			}
			if(d(agent_list.at(i), landmark_list.at(j)) < lm_radius){
				lm_stats.visible(j,i) = 10.*(0.1 - d(agent_list.at(i), landmark_list.at(j)));
				lm_stats.seen(j,i) = 1;
			}
			else
				lm_stats.visible(j,i) = 0;
		}
	}
}

void Environment::update_pipe(){
	for(unsigned int i = 0; i < agent_list.size(); i++){
		for(unsigned int j = 0; j < pipe_list.size(); j++){
			double dis = d(agent_list.at(i), pipe_list.at(j)->in());
			if(dis < 0.1 && !(in_pipe(i,j)) && !(agent_list.at(i)->in_pipe)){
				if(!(agent_list.at(i)->in_pipe))
					printf("In pipe %u\n", j);
				agent_list.at(i)->in_pipe = true;
				in_pipe(i,j) = 1;
				agent_list.at(i)->set_dphi(pipe_angle.at(j));
				agent_list.at(i)->to(pipe_list.at(j)->x0(), pipe_list.at(j)->y0());
			}
			if(d(agent_list.at(i), pipe_list.at(j)->out()) < 0.01 && in_pipe(i,j)){
				printf("Out pipe %u\n", j);
				agent_list.at(i)->in_pipe = false;
				in_pipe(i,j) = 0;
				agent_list.at(i)->set_dphi(new Angle(0.0));
			}

		}
	}
}

void Environment::update_rewards(){
	std::fill(reward.begin(), reward.end(), 0.);
	std::fill(lm_recogn.begin(), lm_recogn.end(), 0.);
	for(unsigned int i = 0; i < agent_list.size(); i++){
		for(unsigned int j = 0; j < goal_list.size(); j++){
			if(d(agent_list.at(i), goal_list.at(j)) < goal_radius && agent_list.at(i)->c()->get_state() == 0){
				reward.at(i) += goal_list.at(j)->a()*(1./goal_radius)*(goal_radius-d(agent_list.at(i), goal_list.at(j)));
				trial_reward.at(i) += reward.at(i);
				total_reward.at(i) += reward.at(i);
				goal_list.at(j)->da();
			}
		}
	}
}

double Environment::x(int i){
	return agent_list.at(i)->x();
}

double Environment::y(int i){
	return agent_list.at(i)->y();
}
