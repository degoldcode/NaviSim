/*
 * navicontrol.cpp
 *
 *  Created on: 29.07.2014
 *      Author: meicron
 */

#include "navicontrol.h"


NaviControl::NaviControl(int num_neurons){
	N = num_neurons;
	pin = new PIN(N, 0.000, 0.02, 0.00);
	gln = new GoalLearning(N, 2, 0.0);
	map = new Map(-20.);

	PI_angle_error = 0.0;
	feedback_error = 0.0;
	goal_fb_error = 0.0;
	map_output = 0.0;
	goal_factor = 0.0;

	PI_angle = 0.0;
	GV_angle = 0.0;
	CM_angle = 0.0;

	stream.open("./data/control.dat");
	t = 0;
}

NaviControl::~NaviControl(){
	delete pin;
	delete gln;
	delete map;
	stream.close();
}

void NaviControl::reset(){
	pin->reset();
	map->reset();
}

void NaviControl::reset_matrices(){
	outputs.reset();
	mu_array.reset();
	gv_array.reset();
	weight.reset();
	out_array.reset();
}

void NaviControl::save_matrices(){
	outputs.save("./data/out.mat", raw_ascii);
	mu_array.save("./data/mu.mat", raw_ascii);
	gv_array.save("./data/gv.mat", raw_ascii);
	out_array.save("./data/gv_out.mat", raw_ascii);
	weight.save("./data/w.mat", raw_ascii);
}

double NaviControl::update(double angle, double speed, double reward){
	vec act_pi = pin->update(angle, speed);
	vec act_gl = gln->update(act_pi, reward);
	map_output = 2.*map->update_map() - 1.;
	if(t%1 == 0)
		update_matrices(act_pi, act_gl);

	PI_angle = bound_angle(pin->max_angle);
	GV_angle = bound_angle(gln->max_angle);
	CM_angle = 0.0;

	feedback_error =  /*gln->act_mu_array(0)*0.5*bound_angle(GV_angle - angle) +*/ inv_angle(PI_angle)-angle;
	goal_factor = (tanh(max(max(gln->w_mu_gv))/N));

	stream	<< map_output << "\t"
			<< PI_angle << "\t"
			<< GV_angle << "\t"
			<< feedback_error << "\t"
			<< goal_factor << "\t"
			<< gln->act_mu_array(0) << "\t"
			<< inv_angle(PI_angle) << endl;

	t++;

	if(gln->act_mu_array(0) == 1.0)
		return 0.05*sin(bound_angle(GV_angle - angle)) + 0.6*map_output;
	else
		return 0.8*map_output + 0.05*sin(feedback_error);
}

void NaviControl::update_matrices(vec PI, vec GL){
	outputs = join_rows(outputs, PI);
	out_array = join_rows(out_array, GL);
	gv_array = join_rows(gv_array, gln->act_gv_array);
	mu_array = join_rows(mu_array, gln->act_mu_array);
	weight = join_rows(weight, gln->w_mu_gv.col(0));
}

void NaviControl::set_inbound(){
	gln->set_mu(0.0, 1.0);
}

void NaviControl::set_outbound(){
	gln->set_mu(1.0, 0.0);
}

double NaviControl::bound_angle(double phi){
	double rphi;
	rphi = phi;
	while(rphi > M_PI)
		rphi -= 2 * M_PI;
	while(rphi < - M_PI)
		rphi += 2 * M_PI;
	return rphi;
}

double NaviControl::inv_angle(double angle){
	return bound_angle(angle - M_PI);
}

double NaviControl::rand(double mean, double stdev){
	static random_device e{};
	static normal_distribution<double> d(mean, stdev);
	return d(e);
}
