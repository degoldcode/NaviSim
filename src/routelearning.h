/*
 * routelearning.h
 *
 *  Created on: Aug 18, 2014
 *      Author: degoldschmidt
 */

#ifndef ROUTELEARNING_H_
#define ROUTELEARNING_H_

#include <armadillo>
#include "pin.h"
#include "circulararray.h"
using namespace arma;

class RouteLearning:
		public CircArray
{
public:
public:
	/**
	 * Constructor
	 *
	 *  @param (int) num_neurons: number of neurons in this array (default: 360)
	 *  @param (int) num_lmr_units: number of landmark recognition units (default: 0)
	 *  @param (double) nnoise: uncorrelated noise in synaptic weights
	 *  @param (double*) forage: pointer to agent's foraging state
	 *  @param (bool) opt_load: true, if loading learned weights from file
	 */
	RouteLearning(int num_neurons, int num_lmr_units, double nnoise, double* forage, bool opt_load=false);

	/**
	 * Destructor
	 *
	 */
	~RouteLearning();

	/**
	 * Get local vector
	 *
	 * @param (int) index: index of synaptic weights
	 * @return (Vec)
	 */
	Vec LV(int index = 0);

	/**
	 * Sets the foraging state
	 *
	 *	@param (double*) state: foraging state
	 * 	@return (void)
	 */
	void set_mu(double* state);

	/**
	 * Updates the route learning circuit
	 *
	 *  @param (double) in_reward: Reward signal
	 *  @param (vec) input_lmr: input from landmark recognition (visual snapshots)
	 *  @return (void)
	 */
	void update(double in_reward, vec input_lmr);

	/**
	 * Updates the weights
	 *
	 *  @return (void)
	 */
	void update_weights();

private:
	PIN * reference_pin;

	vector<Vec> local_vector;                   // Local vectors

	double* foraging_state;
	double learn_rate;
	double reward;
	double expl_rate;
	double neural_noise;
	vec eligibility_lmr;

	mat white_weights;
	mat weight_change;

	bool load_weights;
};



#endif /* ROUTELEARNING_H_ */
