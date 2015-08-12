/*
 * navicontrol.cpp
 *
 *  Created on: 29.07.2014
 *      Author: meicron
 */

#include "controller.h"

Controller::Controller(int num_neurons, double sensory_noise, double leakage, double uncorr_noise){
	numneurons = num_neurons;
	pin = new PIN(numneurons, leakage, sensory_noise, uncorr_noise);
	num_colors = 1;

	gvl = new GoalLearning(numneurons, 0.00, &inward, false);
	gl_array.resize(num_colors);

	rand_m = 0.0;
	pi_m = 0.0;
	gl_m = 0.0;
	rl_m = 0.0;
	rand_w= 1.0;
	pi_w = 1.0;
	gl_w = 1.0;
	rl_w = 1.0;
	output = 0.0;
	inward = 0.0;
	goal_factor = 0.0;

	cGV.resize(num_colors);
	accum_reward = zeros(num_colors);
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

	pin_on = true;
	homing_on = false;
	gvlearn_on = false;
	gvnavi_on = false;


	SILENT = false;
	write = true;
	state_matrc = true;
	stream.open("./data/control.dat");
	r_stream.open("./data/reward.dat");
	lm_stream.open("./data/lm_rec.dat");
	pi_stream.open("./data/pi_activity.dat");

	rx = 0.0;
	ry = 0.0;


	inv_sampling_rate = 1;
	t = 0;
	run = 0;
	t_home = 0;
}

Controller::~Controller() {
	save_matrices();
	delete pin;
	delete gvl;
	stream.close();
	r_stream.close();
	lm_stream.close();
	pi_stream.close();
}

/*double NaviControl::cGV(int i){
	if(i < cGV_angle.size())
		return cGV_angle.at(i);
	else
		return cGV_angle.back();
}*/

/*double NaviControl::expl(){
	return expl_factor(choice);
}*/

double Controller::expl(int i){
	if(i < expl_factor.n_elem)
		return expl_factor(i);
	else
		return expl_factor(0);
}

double Controller::get_state() {
	return inward;
}

Vec Controller::GV(int i){
	if(i < num_colors)
		return gvl->GV(i);
	else
		return gvl->GV(0);
}

Vec Controller::GVc(int i){
	if(i < num_colors)
		return cGV.at(i);
	else
		return cGV.at(0);
}

Vec Controller::HV(){
	return pin->HV();
}

Vec Controller::HVm(){
	return pin->HVm();
}

/*double NaviControl::in_degr(double angle) {
	if (angle > 0.)
		return 180. * angle / M_PI;
	else
		return 180. * (angle + 2 * M_PI) / M_PI;
}*/

int Controller::N(){
	return numneurons;
}

/*void NaviControl::no_write(){
	write = false;
}*/

/*int NaviControl::q(){
	return choice;
}*/

/*double NaviControl::p(int i){
	if(i < prob.n_elem)
		return prob(i);
	else
		return prob(0);
}*/

PIN* Controller::pi(){
	return pin;
}

double Controller::R(int index){
	return accum_reward(index);
}

double Controller::randn(double mean, double stdev) {
	static random_device e { };
	static normal_distribution<double> d(mean, stdev);
	return d(e);
}

/*double NaviControl::randu(double min, double max) {
	static random_device e { };
	static uniform_real_distribution<double> d(min, max);
	return d(e);
}*/

void Controller::reset() {
	pin->reset();
	accum_reward = zeros(num_colors);
	t = 0;
	inward = false;
	run++;
}

/*void NaviControl::reset_matrices() {
	pi_array.reset();
	for(int i = blue; i <= yellow; i++){
		gl_array.at(i).reset();
		gv_weight.at(i).reset();
	}
}*/

void Controller::set_inward(int _time) {
	t_home = _time;
}


void Controller::save_matrices() {
	printf("Save data.\n");
	pi_array.save("./data/mat/pi_activity.mat", raw_ascii);
	gv_array.save("./data/mat/gv_activity.mat", raw_ascii);
//	for(int i = blue; i < num_colors; i++){
//		gl_array.at(i).save("./data/gv.mat", raw_ascii);
//		gv_weight.at(i).save("./data/gv_w.mat", raw_ascii);
//	}
}

void Controller::set_sample_int(int _val){
	inv_sampling_rate = _val;
}

/*void NaviControl::stream_write() {

	//************ Reward stream ************
	if (sum(reward) > 0.0){
		r_stream << HV_x << "\t" << HV_y << "\t";
		for(int i=0; i<num_colors; i++)
			r_stream << GV_x.at(i) << "\t" << GV_y.at(i) << "\t";
	}

	//************ General stream ************
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
}*/

double Controller::update(Angle angle, double speed, double inReward, int color) {
	if(t%inv_sampling_rate == 0 && !SILENT){
		pi_array = join_rows(pi_array, pin->get_output());
		gv_array = join_rows(gv_array, gvl->w(0));
	}
	t++;

	/*** Check, if inward ***/
	if(t > t_home || accum_reward(0) > 3.)
		inward = 1.;

	/*** Random foraging ***/
	rand_m = 4.*randn(0.0, 0.15)*expl_factor(0);

	/*** Path Integration Mechanism ***/
	if(pin_on)
		pin->update(angle, speed);

	if(homing_on && inward!=0.){
		pi_m = 0.5 * ((HV().ang()).i() - angle).S();
		rand_m = 0.0;//0.25;
	}
	else{
		pi_m = 0.;
	}
//	if(HV().len() < 0.2){
//		pi_w -= 0.0001;
//		printf("PI_w = %g\n", pi_w);
//	}

	if(pi_w < 0.)
		pi_w = 0.;

	/*** Global Vector Learning Circuits TODO ***/
	if(gvlearn_on){
		for(int i = 0; i < num_colors; i++){
			if(i == color){
				reward(i) = inReward;
				value(i) = reward(i) + disc_factor * value(i);
				expl_factor(i) = exp(-.5 * value(i));
			}
			else{
				reward(i) = 0.0;
			}
			gvl->update(pin->get_output(), inReward, expl_factor(i));

			cGV.at(i) = (GV(i) - HV());
		}
		if(inward == 0.)
			gl_m = 4.0*(1.-expl_factor(0))*(cGV.at(0).ang() - angle).S();
		else
			gl_m = 0.0;
	}
	accum_reward += reward;




	/*** Navigation Control Output ***/
	output = rand_w*rand_m + pi_w*pi_m + gl_m;
//	if(!inward)
//		output = gl_command + 4.*randn(0.0, 0.15)*expl_factor(0);
//	else
//		output = pi_command;


	return output;
}

/*void NaviControl::update_matrices() {
	pi_array = join_rows(pi_array, pin->rate());
	for(int i = blue; i < gvl.size(); i++){
		gl_array.at(i) = join_rows(gl_array.at(i), gvl.at(i)->rate());
		gv_weight.at(i) = join_rows(gv_weight.at(i), gvl.at(i)->w());
	}
}*/

double Controller::v(int index){
	return value(index);
}
