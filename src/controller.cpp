/*
 * navicontrol.cpp
 *
 *  Created on: 29.07.2014
 *      Author: meicron
 */

#include "navicontrol.h"

NaviControl::NaviControl(int num_neurons, double sensory_noise, double leakage) {
	N = num_neurons;
	pin = new PIN(N, leakage, sensory_noise, 0.00);
	num_colors = 2;
	gvl.resize(num_colors);
	for(int i = 0; i < num_colors; i++)
		gvl.at(i) = new GoalLearning(N, 0.0);

	gl_array.resize(num_colors);
	gv_weight.resize(num_colors);

	pi_command = 0.0;
	gl_command = 0.0;
	rl_command = 0.0;
	foraging_state = 1.0;
	goal_factor = 0.0;

	HV_angle = 0.0;
	HV_len = 0.0;
	HV_x = 0.0;
	HV_y = 0.0;

	GV_angle.resize(num_colors);
	GV_len.resize(num_colors);
	GV_x.resize(num_colors);
	GV_y.resize(num_colors);
	cGV_angle.resize(num_colors);
	reward = zeros(num_colors);
	value = zeros(num_colors);
	dvalue = zeros(num_colors);
	td_error = zeros(num_colors);
	current_goal = 0;

	choice = 0;
	prob = zeros(num_colors);
	act = zeros(num_colors);
	beta = 1.;

	val_discount = 0.99;
	expl_factor = ones(num_colors);

	write = true;
	state_matrc = true;
	stream.open("./data/control.dat");
	r_stream.open("./data/reward.dat");
	lm_stream.open("./data/lm_rec.dat");

	rx = 0.0;
	ry = 0.0;
	inv_sampling_rate = 1;
	t = 0;
	trial_t = 0;
	run = 0;
}

NaviControl::~NaviControl() {
	delete pin;
	for(int i = blue; i < gvl.size(); i++)
		delete gvl.at(i);
	stream.close();
	r_stream.close();
	lm_stream.close();
}

double NaviControl::bound(double angle){
	double rphi = angle;
	while(rphi > M_PI)
		rphi -= 2 * M_PI;
	while(rphi < - M_PI)
		rphi += 2 * M_PI;
	return rphi;
}

double NaviControl::cGV(int i){
	if(i < cGV_angle.size())
		return cGV_angle.at(i);
	else
		return cGV_angle.back();
}

double NaviControl::expl(){
	return expl_factor(choice);
}

double NaviControl::expl(int i){
	if(i < expl_factor.n_elem)
		return expl_factor(i);
	else
		return expl_factor(0);
}

void NaviControl::get_pos(double x, double y) {
	rx = x;
	ry = y;
}

GoalLearning* NaviControl::GV(int i){
	if(i < gvl.size())
		return gvl.at(i);
	else
		return gvl.at(0);
}

PIN* NaviControl::HV(){
	return pin;
}

double NaviControl::in_degr(double angle) {
	if (angle > 0.)
		return 180. * angle / M_PI;
	else
		return 180. * (angle + 2 * M_PI) / M_PI;
}

double NaviControl::inv_angle(double angle) {
	return bound(angle - M_PI);
}

void NaviControl::no_write(){
	write = false;
}

int NaviControl::q(){
	return choice;
}

double NaviControl::p(int i){
	if(i < prob.n_elem)
		return prob(i);
	else
		return prob(0);
}

double NaviControl::rand(double mean, double stdev) {
	static random_device e { };
	static normal_distribution<double> d(mean, stdev);
	return d(e);
}

double NaviControl::randu(double min, double max) {
	static random_device e { };
	static uniform_real_distribution<double> d(min, max);
	return d(e);
}


void NaviControl::reset() {
	pin->reset();
	trial_t = 0;
	run++;
}

void NaviControl::reset_matrices() {
	pi_array.reset();
	for(int i = blue; i <= yellow; i++){
		gl_array.at(i).reset();
		gv_weight.at(i).reset();
	}
}

void NaviControl::set_mode(int in_mode) {
	foraging_state = ((in_mode == 0) ? 1.0 : 0.0);
}


void NaviControl::save_matrices() {
	pi_array.save("./data/out.mat", raw_ascii);
	for(int i = blue; i < num_colors; i++){
		gl_array.at(i).save("./data/gv.mat", raw_ascii);
		gv_weight.at(i).save("./data/gv_w.mat", raw_ascii);
	}
}

void NaviControl::stream_write() {

	//************ Reward stream ************//
	if (sum(reward) > 0.0){
		r_stream << HV_x << "\t" << HV_y << "\t";
		for(int i=0; i<num_colors; i++)
			r_stream << GV_x.at(i) << "\t" << GV_y.at(i) << "\t";
	}

	//************ General stream ************//
	if(t%inv_sampling_rate == 0){
		stream  << t*0.1                                  //1
				<< "\t" << trial_t
				<< "\t" << pi_command
				<< "\t" << gl_command
				<< "\t" << rl_command                     //5
				<< "\t" << in_degr(HV_angle)
				<< "\t" << HV_len
				<< "\t" << HV_x
				<< "\t" << HV_y;

		for(int i=0; i<num_colors; i++){
			stream  << "\t" << in_degr(GV_angle.at(i))   //10-17
			    	<< "\t" << GV_x.at(i)
			    	<< "\t" << GV_y.at(i)
			    	<< "\t" << GV_len.at(i)
			        << "\t" << reward(i);
		}

		stream	<< "\t" << expl_factor
				<< "\t" << current_goal                  //19
				<< endl;
	}
}

double NaviControl::update(double angle, double speed, double inReward, int color) {
	//printf("%4.3f s.\n", t*0.1);
	//if (t % inv_sampling_rate == 0)
		//update_matrices();

	if(write)
		stream_write();

	pin->update(angle, speed);
	HV_angle = pin->avg();
	HV_len = pin->len();
	HV_x = pin->x();
	HV_y = pin->y();


	reward(color) = inReward;
	if(num_colors > 1)
		reward(1-color) = 0.0;
	current_goal = color;
	dvalue(choice) = -value(choice);
	value(choice) = reward(choice) + disc_factor * value(choice);
	dvalue(choice) += value(choice);
	td_error(choice) = reward(choice) + dvalue(choice);
	expl_factor(choice) = exp(-.5 * value(choice));

	for(int i = blue; i < num_colors; i++){
		gvl.at(i)->set_mu(foraging_state);
		gvl.at(i)->update(pin->rate(), reward(color), expl_factor(choice));
		GV_angle.at(i) = gvl.at(i)->avg();
		GV_x.at(i) = gvl.at(i)->x();
		GV_y.at(i) = gvl.at(i)->y();
		cGV_angle.at(i) = atan2(GV_y.at(i) - HV_y, GV_x.at(i) - HV_x);
		goal_factor = 4.0;
	}

	prob = exp(beta*value);
	prob /= sum(exp(beta*value));
	if(num_colors > 1){
		if(trial_t%50==0){
			if(run>100 || trial_t==0){
				if(randu(0.,1.) < prob.at(0))
					choice = blue;
				else
					choice = yellow;
			}
		}
	}
	else
		choice = 0;
	pi_command = sin(inv_angle(HV_angle) - angle);
	gl_command = sin(cGV_angle.at(choice) - angle);


	t++;
	trial_t++;

	if (foraging_state == 1.0){
		return 4.0 * (1. - expl_factor(choice)) * gl_command + expl_factor(choice) * 4. * rand(0., 0.15);
	}
	else
		return 0.5 * rand(0., 0.15) + 4. * pi_command;
}

void NaviControl::update_matrices() {
	pi_array = join_rows(pi_array, pin->rate());
	for(int i = blue; i < gvl.size(); i++){
		gl_array.at(i) = join_rows(gl_array.at(i), gvl.at(i)->rate());
		gv_weight.at(i) = join_rows(gv_weight.at(i), gvl.at(i)->w());
	}
}
