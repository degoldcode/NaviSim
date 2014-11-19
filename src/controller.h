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
//#include "pin.h"
//#include "map.h"
//#include "goallearning.h"
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
	 *
	 */
	Controller(int num_neurons, double sensory_noise, double leakage);

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
	//double bound(double angle);

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
	 * Returns home vector
	 *
	 *  @return (Pointer to CircArray)
	 */
	//PIN* HV();

	/**
	 * Returns inverted angle -> angle - PI (in rad)
	 *
	 *  @param (double) angle: input angle
	 *  @return (double)
	 */
	//double inv_angle(double angle);

	/**
	 * Returns angle in degrees
	 *
	 *  @param (double) angle: input angle
	 *  @return (double)
	 */
	//double in_degr(double angle);

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
	 * Returns a random number drawn from a Gaussian distribution
	 *
	 * @param (double) mean: mean of the distribution (default: 0.0)
	 * @param (double) stdev: width of the distribution (default: 1.0)
	 * @return (double)
	 */
	//double rand(double mean=0.0, double stdev=1.0);

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
	//void reset();

	/**
	 * Resets the state matrices
	 *
	 *  @return (void)
	 */
	//void reset_matrices();

	/**
	 * Saves the state matrices
	 *
	 *  @return (void)
	 */
	//void save_matrices();

	/**
	 * Sets the foraging state
	 *
	 *	@param (int) in_mode: foraging state
	 * 	@return (void)
	 */
	//void set_mode(int in_mode);

	/**
	 * Writes data into files
	 *
	 *  @return (void)
	 */
	//void stream_write();

	/**
	 * Updates the Navigation Controller
	 *
	 *  @param (double) angle: Input angle from compass
	 *  @param (double) speed: Input walking speed from odometry (legged: "differential step counter")
	 *  @param (double) inReward: Reward from environment
	 *  @param (int) inReward: Color of nearest goal
	 *  @return (void)
	 */
	double update(double angle, double speed, double inReward, int color);

	/**
	 * Updates the state matrices
	 *
	 *  @return (void)
	 */
	//void update_matrices();

	double test;


private:

	//************ Navigation Modules & Controller Parameters ************//
	//PIN* pin;
	//vector<GoalLearning*> gvl;
	//RouteLearning* lvl;
	//Map* map;
	double foraging_state;
	double pi_command;
	double gl_command;
	double rl_command;
	double goal_factor;

	//************ Path Integration Parameters ************//
	mat pi_array;
	double HV_angle;
	double HV_len;
	double HV_x;
	double HV_y;

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
	ofstream r_stream;
	ofstream lm_stream;

	bool write;
	bool state_matrc;
	int inv_sampling_rate;	//for activations stored in matrix ([s])

	//************ Global Parameters ************//
	int N;
	double rx;
	double ry;
	int t;
	int trial_t;
	int run;
};
#endif /* NAVICONTROL_H_ */
