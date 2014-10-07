/*****************************************************************************
 *  pin.cpp                                                                  *
 *                                                                           *
 *  Created on:   Oct 07, 2014                                               *
 *  Author:       Dennis Goldschmidt                                         *
 *  Email:        goldschmidtd@ini.phys.ethz.ch                              *
 *                                                                           *
 *                                                                           *
 *  Copyright (C) 2014 by Dennis Goldschmidt                                 *
 *                                                                           *
 *  This file is part of the program NaviSim                                 *
 *                                                                           *
 *  NaviSim is free software: you can redistribute it and/or modify          *
 *  it under the terms of the GNU General Public License as published by     *
 *  the Free Software Foundation, either version 3 of the License, or        *
 *  (at your option) any later version.                                      *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU General Public License for more details.                             *
 *                                                                           *
 *  You should have received a copy of the GNU General Public License        *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
 *                                                                           *
 ****************************************************************************/

#ifndef CIRCULARARRAY_H_
#define CIRCULARARRAY_H_

#include <cmath>
#include <armadillo>
using namespace arma;
using namespace std;


/**
 * Circular Array Class
 *
 * 	This class handles circular array of neurons, where
 * 	each neurons responds to a preferred angle
 *
 */

class CircArray {
public:

	/**
	 * Constructor
	 *
	 *  @param (int) num_neurons: number of neurons in this array (default: 360)
	 *  @param (int) input_dim: number of incoming signals (default: 0)
	 */
	CircArray(int num_neurons=360, int input_dim = 0){
		N = num_neurons;
		M = input_dim;
		preferred_angle.zeros(N);
		for(unsigned int i = 0; i < N; i++)
			preferred_angle(i) = 2. * M_PI * i / N; // Preferred angle of neurons
		max_angle = 0.0;
		max_rate = 0.0;
		avg_angle = 0.0;
		length = 0.0;

		output_rate.zeros(N);
		input_rate.zeros(M);
		input_conns.zeros(N,M);
	};

	/**
	 * Destructor
	 *
	 */
	~CircArray(){

	};

	/**
	 * Returns average angle of maximum firing in the array
	 *
	 *  @return (double)
	 */
	double avg(){
		return avg_angle;
	};

	/**
	 * Returns angle within bounds [-PI,PI]
	 *
	 *  @param (double) angle: input angle
	 *  @return (double)
	 */
	double bound(double angle){
		double rphi = angle;
		while(rphi > M_PI)
			rphi -= 2 * M_PI;
		while(rphi < - M_PI)
			rphi += 2 * M_PI;
		return rphi;
	};

	/**
	 * Returns Gaussian noise with given width
	 *
	 *  @param (double) width: standarad deviation of the normal distribution
	 *  @return (double)
	 */
	double noise(double width);

	/**
	 * Returns the preferred angle of the neuron with maximum firing
	 *
	 *  @return (double)
	 */
	double max(){
		return max_angle;
	};

	/**
	 * Returns the rate of maximum firing neuron
	 *
	 *  @return (double)
	 */
	double maxr(){
		return max_rate;
	};

	/**
	 * Returns the length of represented vector
	 *
	 *  @return (double)
	 */
	double len(){
		return length;
	};

	/**
	 * Returns a vector resulting from linear rectifying transfer function
	 *
	 *  @param (vec) input: input vector to be transfered
	 *  @return (vec)
	 */
	vec lin_rect(vec input){
		vec copy = input;
		for(int i = 0; i < copy.n_rows; i++)
			if(copy(i) < 0.0)
				copy(i) = 0.0;
		return copy;
	};

	/**
	 * Returns the rate vector of the array
	 *
	 *  @return (vec)
	 */
	vec rate();

	/**
	 * Returns the rate of a given neuron of the array
	 *
	 *  @param (int) i: index of neuron
	 *  @return (vec)
	 */
	double rate(int i);

	/**
	 * Resets the activities of the arrays
	 *
	 * @return (void)
	 */
	void reset();

	/**
	 * Updates the activities of the arrays
	 *
	 * @return (void)
	 */
	void update(vec input);

private:
	int N;                                          // Number of neurons
	int M;                                          // Input dimension
	vec preferred_angle;                            // Preferred angle of neurons
	double max_angle;                               // Angle of the maximum-firing neuron
	double max_rate;                                // Maximum rate of neuron array
	double avg_angle;                               // Average position of the maximum firing rate
	double length;                                  // Length of vector = (some scaling factor)*(sum of activities)/N
	const double scale_factor = 7.686168886;        // Scaling factor

	vec output_rate;								// Activity rate of neuron array
	vec input_rate;                                 // Input activity rate to the array
	mat input_conns;                                // incoming connections
};


#endif /* CIRCULARARRAY_H_ */
