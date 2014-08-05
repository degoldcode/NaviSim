/*
 * pin.h
 *
 *  Created on: Jul 23, 2014
 *      Author: degoldschmidt
 */

#ifndef PIN_H_
#define PIN_H_

#include <armadillo>
using namespace std;
using namespace arma;

class PIN {
public:
	PIN(int neurons, double leak, double sensnoise, double neurnoise);
	~PIN();

	void reset();
	vec update(double angle, double speed);
	vec lin_rect(vec input);
	double get_avg_angle(vec input);
	double get_max_angle(vec input);
	double gaussian_noise(double width);
	double bound_PI_angle(double angle);

	vec pref_angle;
	vec act_head_direction;
	vec act_gater;
	vec act_integrator;
	vec act_output;
	mat w_cos;

	double max_angle;
	double avg_angle;

	int N;
	double leak_rate;
	double snoise;
	double nnoise;
	int t;
};



#endif /* PIN_H_ */
