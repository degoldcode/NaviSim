/*
 * navicontrol.cpp
 *
 *  Created on: 29.07.2014
 *      Author: meicron
 */

#include "controller.h"

Controller::Controller(int num_neurons, int _num_gv_units, int _num_lv_units, double sensory_noise, double leakage, double uncorr_noise, double syn_noise, vector<bool> opt_switches){
	homing_on = opt_switches.at(0);
	gvlearn_on = opt_switches.at(1);
	lvlearn_on = opt_switches.at(2);
	SILENT = opt_switches.at(3);

	numneurons = num_neurons;
	pin = new PIN(numneurons, leakage, sensory_noise, uncorr_noise, SILENT);
	pin->VERBOSE=true;

	num_colors = _num_gv_units;
	if(gvlearn_on)
		gvl = new GoalLearning(numneurons, syn_noise, &inward, false, SILENT);
	gl_array.resize(num_colors);

	num_lv_units = _num_lv_units;
	if(lvlearn_on)
		lvl = new RouteLearning(numneurons, num_lv_units, 0.0, &inward, true, SILENT);

	rand_m = 0.0;
	pi_m = 0.0;
	gl_m = 0.0;
	rl_m = 0.0;
	rand_w= 1.0;
	pi_w = 1.0;
	gl_w = 0.0;
	rl_w = 0.0;
	output = 0.0;
	inward = 0.0;
	goal_factor = 0.0;
	expl_beta = 0.1;//0.01;//0.1;	//0.5
	seed = 12345678;

	cGV.resize(num_colors);
	accum_reward = zeros(num_colors);
	cLV.resize(num_lv_units);
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
	beta = 0.5; // 0.5

	val_discount = 0.99;
	expl_factor = ones(num_colors);
	d_expl_factor = zeros(num_colors);

	pin_on = true;
	gvnavi_on = false;
	beta_on = false;

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
	if(!SILENT)
		save_matrices();
	delete pin;
	if(gvlearn_on)
		delete gvl;
	if(lvlearn_on)
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

double Controller::e_beta(){
	return expl_beta;
}

int Controller::get_inward() {
	return t_home;
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

int Controller::K(){
	return num_lv_units;
}

Vec Controller::LV(){
	return lvl->LV();
}

Vec Controller::LV(int i){
	return lvl->LV(i);
}

RouteLearning* Controller::LV_module(){
	return lvl;
}

double Controller::LV_reward(){
	return lvl->R();
}

double Controller::LV_reward(int index){
	return lvl->R(index);
}

Angle Controller::LV_vecavg(int index){
	return lvl->vec_avg(index);
}

double Controller::el_LV_value(int index){
	return lvl->eligibility_value(index);
}

double Controller::LV_value(int index){
	return lvl->lv_value(index);
}

void Controller::LV_value(int index, double value){
	lvl->lv_value(index, value);
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
	if(lvlearn_on){
		lvl->reset_el_lm();
	}
	accum_reward = zeros(num_colors);
	t = 0;
	inward = 0.;
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
	printf("Save matrices.\n");
	pi_array.save("./data/mat/pi_activity.mat", raw_ascii);
//	mat first = pi_array.cols(495,504);
//	mat second = pi_array.cols(995,1004);
//	mat third = pi_array.cols(1495,1504);
//	mat fourth = pi_array.cols(1970,1979);
//	first.save("./data/mat/pi_500.mat", raw_ascii);
//	second.save("./data/mat/pi_1000.mat", raw_ascii);
//	third.save("./data/mat/pi_1500.mat", raw_ascii);
//	fourth.save("./data/mat/pi_2000.mat", raw_ascii);

	gv_array.save("./data/mat/gv_activity.mat", raw_ascii);

	stringstream lv_;
	for(int i = 0; i < lv_array.size(); i++){
		lv_.str(string());
		lv_ << "./data/mat/lv_activity_" << i << ".mat";
		cout << lv_.str() << endl;
		lv_array.at(i).save(lv_.str().c_str(), raw_ascii);
	}

	ref_array.save("./data/mat/ref_activity.mat", raw_ascii);
}

void Controller::set_delta_expl(int _index, double _value, bool _const){
	d_expl_factor(_index) = _value;
	const_expl = _const;
}

void Controller::set_expl(int _index, double _value, bool _const){
	expl_factor(_index) = _value;
	const_expl = _const;
}

void Controller::set_sample_int(int _val){
	if(!SILENT)
		cout << _val << " = sample\n";
	inv_sampling_rate = _val;
}

double Controller::update(Angle angle, double speed, double inReward, vec inLmr, int color) {
	if(t%inv_sampling_rate == 0 && !SILENT){
		pi_array = join_rows(pi_array, pin->get_output());
		if(gvlearn_on){
			gv_array = join_rows(gv_array, gvl->w(0));
		}
		if(lvlearn_on){
			for(int i = 0; i < lv_array.size(); i++)
				lv_array.at(i) = join_rows(lv_array.at(i), lvl->w(i));
			ref_array = join_rows(ref_array, lvl->RefPI());
		}
	}
	t++;

	/*** Check, if inward ***/
	if(t > t_home || accum_reward(0) > 1.){
		inward = 1.;
		//printf("t= %u > %u or sum(R)= %g\n", t, t_home, accum_reward(0));
	}

	/*** Path Integration Mechanism ***/
	if(pin_on)
		pin->update(angle, speed);

	if(gvlearn_on && gl_w > 0.)
		pi_w = HV().len() * (1. - expl_factor(0))*(1.-accu(lv_value));
	else
		pi_w = 0.0;
	pi_m =  ((HV().ang()).i() - angle).S();		//NEW PI COMMAND
	if(homing_on && inward!=0.){
		//printf("this should not be! %g\n", inward);
		pi_w = 0.5;
		pi_m = ((HV().ang()).i() - angle).S();
		rand_m = 0.0;//0.25;
	}
	if(pi_w < 0.)
		pi_w = 0.;
	output_hv = pi_w * pi_m;



	/*** Reward and value update ***/
	if(lvlearn_on){
		for(int i = 0; i < num_lv_units; i++)
			lv_value(i) = 1. - exp(-0.5*lvl->eligibility_value(i));
	}
	delta_beta = mu_beta*((1./expl_beta) + lambda * value(0) * expl_factor(0));
	if(beta_on)
		expl_beta += delta_beta;
	if(expl_factor(0) < 0.0001 && delta_beta < 0.01)
		beta_on = false;

	for(int i = 0; i < num_colors; i++){
		if(i == color){
			reward(i) = inReward;
			if(inward==0.){
				value(i) = (reward(i) /*+ accu(lv_value)*/) + disc_factor * value(i);
				if(!const_expl)
					expl_factor(i) = exp(- expl_beta * value(i));
				else
					expl_factor(i) += d_expl_factor(i);
				expl_factor.elem( find(expl_factor > 1.) ).ones();    //clip expl
				expl_factor.elem( find(expl_factor < 0.) ).zeros();   //clip expl
			}
		}
		else{
			reward(i) = 0.0;
		}
	}
	accum_reward += reward;

	/*** Global Vector Learning Circuits TODO ***/
	if(gvlearn_on){
		for(int i = 0; i < num_colors; i++){
			gvl->update(pin->get_output(), reward(i), expl_factor(i));
			cGV.at(i) = (GV(i) - HV());
		}

		gl_w = (1. - inward)*GV(0).len() * (1.-expl_factor(0));
		gl_m = (GV(0).ang() - angle).S();			//NEW GV COMMAND
	}
	stream << cGV.at(0).ang().deg() << endl;
	output_gv = gl_w * gl_m;

	/*** Local Vector Learning Circuits TODO ***/
	if(lvlearn_on){

		lvl->update(angle, speed, inReward, inLmr);

		rl_m = 0.0;
		rl_w = (1. - inward);
		for(int i = 0; i < num_lv_units; i++){
			//cLV.at(i) = (LV(i) - HV());
			if(inward == 0. && lvl->el_lm(i) > 0.1){
				gl_w = 0.0;
				pi_w = 0.0;
				rl_m += lv_value(i) * (LV().len()*(LV().ang() - angle).S() + RV().len()*(RV().ang().i() - angle).S());
			}
		}
		output_lv = rl_w * rl_m;
	}

	/*** Random foraging ***/
	rand_w = (1. - inward)*0.6*expl_factor(0)*(1.-accu(lv_value));
	rand_m = randn(0.0, 1.);
	if(inward == 1)
		rand_m = 0.;
	output_rand = rand_w * rand_m;

	/*** Navigation Control Output ***/
	output = output_rand + output_hv + 0.0 + output_lv;

	return output;
}

double Controller::v(int index){
	return value(index);
}
