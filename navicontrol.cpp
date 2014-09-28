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
	gln = new GoalLearning(N, 2, 0.0);
	rln = new RouteLearning(N, 1, 0.0);
	map = new Map(-20.);

	PI_angle_error = 0.0;
	feedback_error = 0.0;
	goal_fb_error = 0.0;
	map_output = 0.0;
	goal_factor = 0.0;

	PI_max_angle = 0.0;
	PI_avg_angle = 0.0;
	PI_x = 0.0;
	PI_y = 0.0;

	num_colors = 2;
	GV_angle.resize(num_colors);
	GV_x.resize(num_colors);
	GV_y.resize(num_colors);
	cGV_angle.resize(num_colors);


	LV_angle = 0.0;
	LV_x = 0.0;
	LV_y = 0.0;
	cLV_angle = 0.0;
	CM_angle = 0.0;

	lm_lowpass = 0.0;
	reward = zeros(num_colors);
	value = zeros(num_colors);
	dvalue = zeros(num_colors);
	td_error = zeros(num_colors);

	prob = zeros(num_colors);
	act = zeros(num_colors);
	beta = .01;

	val_discount = 0.99;
	disc_factor = 0.999;	//0.999
	expl_factor = ones(num_colors);
	foraging = 1.0;

	stream.open("./data/control.dat");
	r_stream.open("./data/reward.dat");
	lm_stream.open("./data/lm_rec.dat");
	inv_sampling_rate = 10000;
	t = 0;
	trial_t = 0;
	run = 0;
}

NaviControl::~NaviControl() {
	delete pin;
	delete gln;
	delete rln;
	delete map;
	stream.close();
	r_stream.close();
	lm_stream.close();
}

void NaviControl::reset() {
	pin->reset();
	map->reset();
	start_fixed = true;
	trial_t = 0;
	run++;
	reset_matrices();
}

void NaviControl::reset_matrices() {
	outputs.reset();
	mu_array.reset();
	gv_array.reset();
	gv_weight.reset();
	lv_weight.reset();
	out_array.reset();
	ref_array.reset();
	lv_array.reset();
}

void NaviControl::save_matrices() {
	outputs.save("./data/out.mat", raw_ascii);
	//mat mu_new = mu_array.col(0);
	//mu_new.save("./data/mu.mat", raw_ascii);
	gv_array.save("./data/gv.mat", raw_ascii);
	ref_array.save("./data/ref.mat", raw_ascii);
	lv_array.save("./data/lv.mat", raw_ascii);
	out_array.save("./data/gv_out.mat", raw_ascii);
	gv_weight.save("./data/gv_w.mat", raw_ascii);
	lv_weight.save("./data/lv_w.mat", raw_ascii);
}

double NaviControl::update(double angle, double speed, double inReward, int color) {
	//printf("%4.3f s.\n", t*0.1);
	reward(color) = inReward;
	reward(1-color) = 0.0;
	if (lm_lowpass > 0.0)
		lm_stream << t << "\t" << rx << "\t" << ry << "\t" << color << "\t"
				<< lm_lowpass << "\t" << reward(0) << "\t" << reward(1) << endl;

	if (sum(reward) > 0.0){
		r_stream << PI_x << "\t" << PI_y << "\t";
		for(int i=0; i<num_colors; i++)
			r_stream << GV_x.at(i) << "\t" << GV_y.at(i) << "\t";
		r_stream << LV_x << "\t" << LV_y << endl;
	}

	//if(t%inv_sampling_rate == 0)
	stream << t << "\t" << map_output << "\t" << PI_max_angle << "\t"
			<< PI_avg_angle << "\t" << in_degr(PI_max_angle) << "\t"
			//5
			<< in_degr(PI_avg_angle) << "\t" << pin->length << "\t" << PI_x << "\t" << PI_y;
			for(int i=0; i<num_colors; i++)
				stream  << "\t" << GV_x.at(i)
						<< "\t" << GV_y.at(i)
						<< "\t" << GV_angle.at(i)
						<< "\t" << in_degr(GV_angle.at(0));

			//10
			stream << "\t" << feedback_error << "\t"
			<< goal_factor << "\t" << gln->act_mu_array(0) << "\t"
			<< inv_angle(PI_avg_angle) << "\t"
			//15
			<< expl_factor << "\t" << color << "\t" << lm_lowpass << "\t"
			<< reward(0) << "\t" << reward(1) << "\t" << LV_angle << "\t"						//20
			<< in_degr(LV_angle) << "\t" << endl;

	//	if(reward > 0.0){
	//		gln->sum_length += pin->length;
	//	}



	dvalue(choice) = -value(choice);
	value(choice) = reward(choice) + disc_factor * value(choice);
	dvalue(choice) += value(choice);
	td_error(choice) = reward(choice) + dvalue(choice);
	expl_factor(choice) = exp(-.5 * value(choice));

	gln->set_mu(1. - foraging, 1 - color);
	gln->set_mu(foraging, color);
	vec act_pi = pin->update(angle, speed);
	vec act_gl = gln->update(act_pi, reward(color), expl_factor(choice));
	vec act_rl = zeros(N);//rln->update(act_pi, reward, lm_recogn, angle, speed);

	if (t % inv_sampling_rate == 0)
		update_matrices(act_pi, act_gl, act_rl);

	map_output = 2. * map->update_map() - 1.;

	PI_max_angle = bound_angle(pin->max_angle);
	PI_avg_angle = bound_angle(pin->avg_angle);
	PI_x = pin->length * cos(PI_avg_angle);
	PI_y = pin->length * sin(PI_avg_angle);
	//	if(t%100==0)
	//		cout << t << " " << gln->var << endl;
	for(int i=0; i<num_colors; i++){
		GV_angle.at(i) = bound_angle(gln->angle(i));
		GV_x.at(i) = gln->len(i) * cos(GV_angle.at(i));
		GV_y.at(i) = gln->len(i) * sin(GV_angle.at(i));
		cGV_angle.at(i) = atan2(GV_y.at(i) - PI_y, GV_x.at(i) - PI_x);
		goal_factor = 4.0;//*(tanh(max(max(gln->w_mu_gv.col(i)))/N));
	}

	LV_angle = bound_angle(rln->max_angle);
	LV_x = rln->length * cos(LV_angle);
	LV_y = rln->length * sin(LV_angle);

	//if(sqrt(pow(GV_x-PI_x,2)+pow(GV_y-PI_y,2))>0.2)
	//if((GV_x-PI_x)*cos(angle)+(GV_y-PI_y)*sin(angle)>0.2)
	cLV_angle = atan2(LV_y - PI_y, LV_x - PI_x);
	CM_angle = 0.0;

	feedback_error = inv_angle(PI_avg_angle) - angle;
	//goal_factor = 10.0 * (tanh(max(max(rln->w_lmr_lv.col(0))) / N));

	lm_lowpass = gln->act_mu_array(0) * (0.05 *0.0 + 0.95 * lm_lowpass);
	//gln->learn_rate = expl_factor;

	/*	if(sqrt(rx*rx + ry*ry) < 1.11 && start_fixed){
	 start_fixed = false;
	 return 4.*sin(atan2(1., 0.5) - angle);
	 }*/

	//ACTION SELECTION
	vec act_rate = zeros(num_colors);
	if(sum(reward)>0.0)
		act_rate(color) = 0.1;
	act = act + act_rate%(value);//-act);
	prob = exp(beta*value);
	prob /= sum(exp(beta*value));
	if(trial_t%50==0){
		if(run>100 || trial_t==0){
			if(randu(0.,1.) < prob.at(0))
				choice = blue;
			else
				choice = yellow;
		}
	}

	t++;
	trial_t++;

	if (foraging == 1.0){
		return /*0.05*/(1. - expl_factor(choice)) * goal_factor * sin(cGV_angle.at(choice) - angle) + expl_factor(choice) * 10. * rand(0., 0.15);
	}
	else
		return 0.5 * rand(0., 0.15) + 4. * sin(feedback_error);
}

void NaviControl::update_matrices(vec PI, vec GL, vec RL) {
	outputs = join_rows(outputs, PI);
	out_array = join_rows(out_array, GL);
	lv_array = join_rows(lv_array, RL);
	ref_array = join_rows(ref_array, rln->act_ref_array);
	gv_array = join_rows(gv_array, gln->act_gv_array);
	mu_array = join_rows(mu_array, gln->act_mu_array);
	gv_weight = join_rows(gv_weight, gln->w_mu_gv.col(0));
	lv_weight = join_rows(lv_weight, rln->w_lmr_lv.col(0));
}

void NaviControl::set_inbound() {
	foraging = 0.0;
}

void NaviControl::set_outbound() {
	foraging = 1.0;
}

double NaviControl::bound_angle(double phi) {
	double rphi;
	rphi = phi;
	while (rphi > M_PI)
		rphi -= 2 * M_PI;
	while (rphi < - M_PI)
		rphi += 2 * M_PI;
	return rphi;
}

double NaviControl::in_degr(double angle) {
	if (angle > 0.)
		return 180. * angle / M_PI;
	else
		return 180. * (angle + 2 * M_PI) / M_PI;
}

double NaviControl::inv_angle(double angle) {
	return bound_angle(angle - M_PI);
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

void NaviControl::get_pos(double x, double y) {
	rx = x;
	ry = y;
}

double NaviControl::get_HV(){
	return PI_avg_angle;
}

double NaviControl::get_expl(){
	return expl_factor(choice);
}
