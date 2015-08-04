/*
 * navicontrol.h
 *
 *  Created on: 29.07.2014
 *      Author: meicron
 */

#ifndef NAVICONTROL_H_
#define NAVICONTROL_H_


#include <armadillo>
#include <cmath>
#include <vector>
#include "pin.h"
#include "geom.h"
#include "goallearning.h"
//#include "routelearning.h"
using namespace std;
using namespace arma;

/*** Goal types (colors) ***/
enum{blue,yellow};


/**
 * Navigation Controller Class
 *
 * 	This class controls agents in the
 * 	environment
 *
 */

class Controller {
public:

	/**
	 * Constructor
	 *
	 *	@param (int) num_neurons: number of neurons
	 *	@param (double) sensory_noise: sensory noise to PI
	 *	@param (double) leakage: leakage of PI memory
	 *	@param (double) uncorr_noise: uncorrelated noise at input layer (default: 0.)
	 *
	 */
	Controller(int num_neurons, double sensory_noise, double leakage, double uncorr_noise= 0.);

	/**
	 * Destructor
	 *
	 */
	~Controller();

	/**
	 * Returns angle within bounds [-PI,PI]
	 *
	 *  @param (double) angle: input angle
	 *  @return (double)
	 */
	double bound(double angle);

	/**
	 * Returns current goal vector angle of goal i
	 *
	 *  @param (int) i: goal index
	 *  @return (double)
	 */
	//double cGV(int i);

	/**
	 * Returns exploration rate of chosen goal
	 *
	 *  @return (double)
	 */
	//double expl();

	/**
	 * Returns exploration rate of goal i
	 *
	 *  @param (double) i: goal index
	 *  @return (double)
	 */
	//double expl(int i);

	/**
	 * Sets real position of agent
	 *
	 *  @param (double) x: x position of agent
	 *  @param (double) y: y position of agent
	 *  @return (void)
	 */
	//void get_pos(double x, double y);

	/**
	 * Returns global vector of goal i
	 *
	 *  @param (int) i: goal index
	 *  @return (Pointer to CircArray)
	 */
	//GoalLearning* GV(int i);

	/**
	 * Returns home vector using average
	 *
	 *  @return (Pointer to Vector)
	 */
	Vec HV();

	/**
	 * Returns home vector using maximum
	 *
	 *  @return (Pointer to Vector)
	 */
	Vec HVm();

	/**
	 * Returns inverted angle -> angle - PI (in rad)
	 *
	 *  @param (double) angle: input angle
	 *  @return (double)
	 */
	double inv_angle(double angle);

	/**
	 * Returns angle in degrees
	 *
	 *  @param (double) angle: input angle
	 *  @return (double)
	 */
	//double in_degr(double angle);


	/**
	 * Return number of neurons per circular array
	 *
	 * return (int)
	 */
	int N();

	/**
	 * Sets write option to false -> no writing to file
	 *
	 *  @return (void)
	 */
	//void no_write();

	/**
	 * Returns agent's goal choice
	 *
	 *  @return (int)
	 */
	//int q();

	/**
	 * Returns choice probability of goal i
	 *
	 *  @param (int) i: goal index
	 *  @return (double)
	 */
	//double p(int i);


	/**
	 * Return PI module
	 *
	 * @return (PIN*)
	 */
	PIN* pi();

	/**
	 * Returns a random number drawn from a Gaussian distribution
	 *
	 * @param (double) mean: mean of the distribution (default: 0.0)
	 * @param (double) stdev: width of the distribution (default: 1.0)
	 * @return (double)
	 */
	double randn(double mean=0.0, double stdev=1.0);

	/**
	 * Returns a random number drawn from a uniform distribution
	 *
	 * @param (double) min: lower bound of the distribution (default: 0.0)
	 * @param (double) max: upper bound of the distribution (default: 1.0)
	 * @return (double)
	 */
	//double randu(double min=0.0, double max=1.0);

	/**
	 * Resets the controller for subsequent runs
	 *
	 *  @return (void)
	 */
	void reset();

	/**
	 * Resets the state matrices
	 *
	 *  @return (void)
	 */
	//void reset_matrices();

	/**
	 * Save activity/weight matrices
	 *
	 *  @return (void)
	 */
	void save_matrices();

	/**
	 * Sets the foraging state
	 *
	 *	@param (int) in_mode: foraging state
	 * 	@return (void)
	 */
	void set_inward(bool in_mode);

	/**
	 * Writes data into files
	 *
	 *  @return (void)
	 */
	//void stream_write();


	/**
	 * Set sample interval for how often activity data gets sampled
	 *
	 *  @param (int) _val: sample interval
	 *  @return (void)
	 */
	void set_sample_int(int _val);

	/**
	 * Updates the Navigation Controller
	 *
	 *  @param (double) angle: Input angle from compass
	 *  @param (double) speed: Input walking speed from odometry (legged: "differential step counter")
	 *  @param (double) inReward: Reward from environment
	 *  @param (int) inReward: Color of nearest goal
	 *  @return (void)
	 */
	double update(Angle angle, double speed, double inReward, int color);

	/**
	 * Updates the state matrices
	 *
	 *  @return (void)
	 */
	//void update_matrices();



private:

	//************ Navigation Modules & Controller Parameters ************//
	PIN* pin;
	vector<GoalLearning*> gvl;
	//RouteLearning* lvl;
	//Map* map;
	bool inward;
	double pi_m;
	double gl_m;
	double rl_m;
	double output;
	double goal_factor;

	//************ Path Integration Parameters ************//
	mat pi_array;

	//************ Goal Learning Parameters ************//
	vector<mat> gl_array;
	vector<mat> gv_weight;
	vector<double> GV_angle;
	vector<double> GV_len;
	vector<double> GV_x;
	vector<double> GV_y;
	vector<double> cGV_angle;
	vec reward;
	vec td_error;
	vec value;
	vec dvalue;
	double val_discount;
	const double disc_factor = 0.999;
	vec expl_factor;
	int current_goal;

	//************ Decision Making Parameters ************//
	int num_colors;
	vec prob;
	vec act;
	int choice;
	double beta;

	//************ Options ************//

	//************ Recording Parameters ************//
	ofstream stream;
	ofstream pi_stream;
	ofstream r_stream;
	ofstream lm_stream;

	//************ Global Parameters ************//
	int numneurons;
	double rx;
	double ry;
	int t;
	int trial_t;
	int run;

public:
	bool write;
	bool SILENT;			// no activity matrices sampling
	bool state_matrc;
	int inv_sampling_rate;	//for activations stored in matrix ([s])
};
#endif /* NAVICONTROL_H_ */
