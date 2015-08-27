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

	gvl = new GoalLearning(numneurons, 0.0, &inward, false);
	gl_array.resize(num_colors);

	num_lv_units = 2;
	lvl = new RouteLearning(numneurons, num_lv_units, 0.0, &inward, false);

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
	expl_beta = 0.5;
	seed = 12345678;

	cGV.resize(num_colors);
	cLV.resize(num_colors);
	accum_reward = zeros(num_colors);
	lv_value = zeros(num_lv_units);
	lv_array.resize(num_lv_units);
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
	lvlearn_on = false;


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
	delete lvl;
	stream.close();
	r_stream.close();
	lm_stream.close();
	pi_stream.close();
}

double Controller::el_lm(int index){
	return lvl->el_lm(index);
}

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

GoalLearning* Controller::GV_module(){
	return gvl;
}

Angle Controller::GV_vecavg(){
	return gvl->vec_avg();
}

Vec Controller::HV(){
	return pin->HV();
}

Vec Controller::HVm(){
	return pin->HVm();
}

Vec Controller::LV(int i){
	return lvl->LV(i);
}

RouteLearning* Controller::LV_module(){
	return lvl;
}

double Controller::LV_value(int index){
	return lvl->value_lm(index);
}

double Controller::LV_value_raw(int index){
	return lvl->value_lm_raw(index);
}

int Controller::N(){
	return numneurons;
}

PIN* Controller::pi(){
	return pin;
}

double Controller::R(int index){
	return accum_reward(index);
}

Vec Controller::RV(){
	return lvl->RV();
}

double Controller::randn(double mean, double stdev) {
//	static random_device e { };
//	static normal_distribution<double> d(mean, stdev);
//	return d(e);
	boost::mt19937 rng; // I don't seed it on purpouse (it's not relevant)

	rng.seed((++seed) + time(NULL));
	boost::normal_distribution<> nd(mean, stdev);
	boost::variate_generator<boost::mt19937&,
	boost::normal_distribution<> > var_nor(rng, nd);
	return var_nor();
}

void Controller::reset() {
	pin->reset();
	lvl->reset_el_lm();
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

	stringstream lv_;
	for(int i = 0; i < lv_array.size(); i++){
		lv_.str(string());
		lv_ << "./data/mat/lv_activity_" << i << ".mat";
		cout << lv_.str() << endl;
		lv_array.at(i).save(lv_.str().c_str(), raw_ascii);
	}

	ref_array.save("./data/mat/ref_activity.mat", raw_ascii);
//	for(int i = blue; i < num_colors; i++){
//		gl_array.at(i).save("./data/gv.mat", raw_ascii);
//		gv_weight.at(i).save("./data/gv_w.mat", raw_ascii);
//	}
}

void Controller::set_sample_int(int _val){
	cout << _val << " = sample\n";
	inv_sampling_rate = _val;
}

double Controller::update(Angle angle, double speed, double inReward, vec inLmr, int color) {
	if(t%inv_sampling_rate == 0 && !SILENT){
		pi_array = join_rows(pi_array, pin->get_output());
		gv_array = join_rows(gv_array, gvl->w(0));
		for(int i = 0; i < lv_array.size(); i++)
			lv_array.at(i) = join_rows(lv_array.at(i), lvl->w(0));
		ref_array = join_rows(ref_array, lvl->RefPI());
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

	for(int i = 0; i < num_lv_units; i++)
		lv_value(i) = 1. - exp(-0.01*lvl->value_lm(i));

	/*** Global Vector Learning Circuits TODO ***/
	if(gvlearn_on){
		for(int i = 0; i < num_colors; i++){
			if(i == color){
				reward(i) = inReward;
				value(i) = (reward(i) + accu(lv_value)) + disc_factor * value(i);
				expl_factor(i) = exp(- expl_beta * value(i));
			}
			else{
				reward(i) = 0.0;
			}
			gvl->update(pin->get_output(), lv_value(0) /*reward(i)*/, expl_factor(i));

			cGV.at(i) = (GV(i) - HV());
		}
		if(inward == 0.)
			gl_m = 4.0*(1.-expl_factor(0))*(cGV.at(0).ang() - angle).S();
		else
			gl_m = 0.0;
	}
	stream << cGV.at(0).ang().deg() << endl;
	gl_w = 1.0;
	accum_reward += reward;

	/*** Local Vector Learning Circuits TODO ***/
	if(lvlearn_on){
		//rl_w = 1. - exp(-LV(0).len());
		lvl->update(angle, speed, inReward, inLmr);

		cLV.at(0) = (LV(0) - HV());

		double route_factor = 2.*(lvl->el_lm(0)-0.5);
		if(route_factor < 0.0)
			route_factor = 0.0;
		route_factor = 1.0;
		if(inward == 0. && lvl->el_lm(0) > 0.5){
			gl_w = 0.0;
			rl_m = 4.0*route_factor*(1.-expl_factor(0))*(LV(0).ang() - angle).S();//4.0*(1.-expl_factor(0))*(LV(0).ang() - angle).S();
		}
		else
			rl_m = 0.0;
	}



	/*** Navigation Control Output ***/
	output = rand_w*rand_m + pi_w*pi_m + gl_w*gl_m + rl_w*rl_m;
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
