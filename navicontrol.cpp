/*
 * navicontrol.cpp
 *
 *  Created on: 29.07.2014
 *      Author: meicron
 */

#include "navicontrol.h"


NaviControl::NaviControl(int num_neurons){
	N = num_neurons;
	pin = new PIN(N, 0.0000, 0.02, 0.00);
	gln = new GoalLearning(N, 2, 0.0);
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
	GV_angle = 0.0;
	GV_x = 0.0;
	GV_y = 0.0;
	cGV_angle = 0.0;
	CM_angle = 0.0;

	accu_reward = 0.0;
	disc_factor = 0.999;
	expl_factor = 1.0;

	stream.open("./data/control.dat");
	r_stream.open("./data/reward.dat");
	inv_sampling_rate = 500;
	t = 0;
}

NaviControl::~NaviControl(){
	delete pin;
	delete gln;
	delete map;
	stream.close();
	r_stream.close();
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
	if(reward>0.0)
		r_stream << PI_x << "\t"
					<< PI_y << "\t"
					<< GV_x << "\t"
					<< GV_y << endl;

	if(t%inv_sampling_rate == 0)
		stream	<< t << "\t"
				<< map_output << "\t"
				<< PI_max_angle << "\t"
				<< PI_avg_angle << "\t"
				<< in_degr(PI_max_angle) << "\t"
				<< in_degr(PI_avg_angle) << "\t"
				<< pin->length << "\t"
				<< PI_x << "\t"
				<< PI_y << "\t"
				<< GV_angle << "\t"
				<< in_degr(GV_angle) << "\t"
				<< feedback_error << "\t"
				<< goal_factor << "\t"
				<< gln->act_mu_array(0) << "\t"
				<< inv_angle(PI_avg_angle) << "\t"
				<< expl_factor << endl;

//	if(reward > 0.0){
//		gln->sum_length += pin->length;
//	}

	vec act_pi = pin->update(angle, speed);
	vec act_gl = gln->update(act_pi, reward);

	if(t%inv_sampling_rate == 0)
			update_matrices(act_pi, act_gl);

	map_output = 2.*map->update_map() - 1.;

	PI_max_angle = bound_angle(pin->max_angle);
	PI_avg_angle = bound_angle(pin->avg_angle);
	PI_x = pin->length * cos(PI_avg_angle);
	PI_y = pin->length * sin(PI_avg_angle);
//	if(t%100==0)
//		cout << t << " " << gln->var << endl;
	GV_angle = bound_angle(gln->max_angle);
	GV_x = gln->length * cos(GV_angle);
	GV_y = gln->length * sin(GV_angle);

	//if(sqrt(pow(GV_x-PI_x,2)+pow(GV_y-PI_y,2))>0.2)
	//if((GV_x-PI_x)*cos(angle)+(GV_y-PI_y)*sin(angle)>0.2)
		cGV_angle = atan2(GV_y-PI_y, GV_x-PI_x);
	CM_angle = 0.0;

	feedback_error = inv_angle(PI_avg_angle)-angle;
	goal_factor = 4.0*(tanh(max(max(gln->w_mu_gv.col(0)))/N));

	accu_reward = reward + disc_factor*accu_reward;
	expl_factor = exp(-accu_reward);

	t++;

	if(gln->act_mu_array(0) == 1.0)
		return /*0.05*/ (1.-expl_factor)*goal_factor*sin(cGV_angle - angle) + expl_factor*10.*rand(0., 0.15);//+ 1.5*map_output; //rand(0., 0.08); //
	else
		return 0.5*rand(0., 0.15) + 4.*sin(feedback_error);
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

double NaviControl::in_degr(double angle){
	if(angle > 0.)
		return 180.*angle/M_PI;
	else
		return 180.*(angle+2*M_PI)/M_PI;
}

double NaviControl::inv_angle(double angle){
	return bound_angle(angle - M_PI);
}

double NaviControl::rand(double mean, double stdev){
	static random_device e{};
	static normal_distribution<double> d(mean, stdev);
	return d(e);
}
