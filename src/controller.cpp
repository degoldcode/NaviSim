/*
 * navicontrol.cpp
 *
 *  Created on: 29.07.2014
 *      Author: meicron
 */

#include "controller.h"

Controller::Controller(int num_neurons, double sensory_noise, double leakage, double uncorr_noise, double syn_noise, vector<bool> opt_switches){
	numneurons = num_neurons;
	homing_on = opt_switches.at(0);
	gvlearn_on = opt_switches.at(1);
	lvlearn_on = opt_switches.at(2);
	pin = new PIN(numneurons, leakage, sensory_noise, uncorr_noise);
	num_colors = 1;

	if(gvlearn_on)
		gvl = new GoalLearning(numneurons, syn_noise, &inward, false);
	gl_array.resize(num_colors);

	num_lv_units = 3;
	if(lvlearn_on)
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
	expl_beta = 1.;//0.1;	//0.5
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

	pin_on = true;
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
	if(lvlearn_on)
		lvl->reset_el_lm();
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

void Controller::set_sample_int(int _val){
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

	/*** Random foraging ***/
	rand_m = 4.*0.15*randn(0.0, 1.)*expl_factor(0);

	/*** Path Integration Mechanism ***/
	if(pin_on)
		pin->update(angle, speed);

	pi_m =  HV().len() * (1. - expl_factor(0)) * ((HV().ang()).i() - angle).S();		//NEW PI COMMAND
	if(homing_on && inward!=0.){
		//printf("this should not be! %g\n", inward);
		pi_m = 0.5 * ((HV().ang()).i() - angle).S();
		rand_m = 0.0;//0.25;
	}
//	else{
//		pi_m = 0.;
//	}
//	if(HV().len() < 0.2){
//		pi_w -= 0.0001;
//		printf("PI_w = %g\n", pi_w);
//	}

	if(pi_w < 0.)
		pi_w = 0.;

	if(lvlearn_on){
		for(int i = 0; i < num_lv_units; i++)
			lv_value(i) = 1. - exp(-0.01*lvl->value_lm(i));
	}

	/*** Global Vector Learning Circuits TODO ***/
	if(gvlearn_on){
		for(int i = 0; i < num_colors; i++){
			if(i == color){
				reward(i) = inReward;
				if(inward==0.){
					value(i) = (reward(i) + accu(lv_value)) + disc_factor * value(i);
					expl_factor(i) = exp(- expl_beta * value(i));
				}
			}
			else{
				reward(i) = 0.0;
			}
			gvl->update(pin->get_output(), lv_value(0)/*reward(i)*/, expl_factor(i));

			cGV.at(i) = (GV(i) - HV());
		}

		gl_m = GV(0).len() * (1.-expl_factor(0))*(1. - inward)*(GV(0).ang() - angle).S();			//NEW GV COMMAND
//		if(inward == 0.)
//			gl_m = (1.-expl_factor(0))*(cGV.at(0).ang() - angle).S();
//		else
//			gl_m = 0.0;
	}
	stream << cGV.at(0).ang().deg() << endl;
	//gl_w = 0.0;
	accum_reward += reward;

	/*** Local Vector Learning Circuits TODO ***/
	if(lvlearn_on){
		if(cGV.at(0).len() < 0.5)
			gl_w = 0.0;

		//rl_w = 1. - exp(-LV(0).len());
		lvl->update(angle, speed, inReward, inLmr);

		vector<double> route_factor;
		route_factor.resize(num_lv_units);
		rl_m = 0.0;
		for(int i = 0; i < num_lv_units; i++){
			cLV.at(i) = (LV(i) - HV());


			route_factor.at(i)= 2.*(lvl->el_lm(i)-0.5);
			if(route_factor.at(i) < 0.0)
				route_factor.at(i) = 0.0;
			route_factor.at(i) = 1.0;
			if(inward == 0. && lvl->el_lm(i) > 0.5){
				gl_w = 0.0;
				rl_m += 4.0*route_factor.at(i)*(1.-expl_factor(0))*(LV(i).ang() - angle).S();//4.0*(1.-expl_factor(0))*(LV(0).ang() - angle).S();
			}
		}
	}




	/*** Navigation Control Output ***/
	output = rand_w*rand_m + pi_w*pi_m + gl_w*gl_m + rl_w*rl_m;

	return output;
}

double Controller::v(int index){
	return value(index);
}
