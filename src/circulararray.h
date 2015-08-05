/*****************************************************************************
 *  circulararray.h                                                          *
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
#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include "geom.h"
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
		K = input_dim;
		preferred_angle.zeros(N);
		for(unsigned int i = 0; i < N; i++)
			preferred_angle(i) = 2. * M_PI * i / N; // Preferred angle of neurons
		//cout << preferred_angle << endl;
		max_rate = 0.0;
		length = 0.0;
		type = 0;
		seed = 0;

		output_rate.zeros(N);
		input_rate.zeros(K);
		input_conns.zeros(N,K);
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
	 *  @return (Angle)
	 */
	Angle avg(){
		return avg_angle;
	};

	/**
	 * Returns cosine kernel
	 *
	 *  @return (mat)
	 */
	mat cos_kernel(){
		mat w_cos = zeros<mat>(N,K);
		for(int i = 0; i < N; i++)
				for(int j = 0; j < N; j++)
					w_cos(i,j) = cos(preferred_angle(i) - preferred_angle(j));
		return w_cos;
	};

	/**
	 * Returns Gaussian noise with given width
	 *
	 *  @param (double) width: standarad deviation of the normal distribution
	 *  @return (double)
	 */
	double noise(double width){
		if(width > 0.0){
		    std::random_device rd;
		    std::mt19937 gen(rd());
			static normal_distribution<double> d(0., width);
			return d(rd);
		}
		else
			return 0.;
	};
	double boost_noise(double width){
		boost::mt19937 rng; // I don't seed it on purpouse (it's not relevant)

		rng.seed((++seed) + time(NULL));
		boost::normal_distribution<> nd(0.0, width);
		boost::variate_generator<boost::mt19937&,
		boost::normal_distribution<> > var_nor(rng, nd);
		return var_nor();
	}

	/**
	 * Returns the preferred angle of the neuron with maximum firing
	 *
	 *  @return (Angle)
	 */
	Angle max(){
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
	vec rate(){
		return output_rate;
	};

	/**
	 * Returns the rate of a given neuron of the array
	 *
	 *  @param (int) i: index of neuron
	 *  @return (vec)
	 */
	double rate(int i){
		return output_rate(i);
	};

	/**
	 * Resets the activities of the array
	 *
	 * @return (void)
	 */
	void reset(){
		input_rate.zeros();
		output_rate.zeros();
	};

	/**
	 * Sets the incoming connections to the array
	 *
	 * @return (void)
	 */
	void set_w(mat weight){
		input_conns = weight;
	};

	/**
	 * Updates the average angle of maximum firing in the array
	 *
	 * @param (vec) input: vector
	 * @return (void)
	 */
	void update_avg(vec input){
		double sum_act = 0.0;
		double output = 0.0;
		int _start = 0;
		int _end = 0;

		for(int i = 0; i < N; i++){
			if(input(i%N) > 0. && input((i+1)%N) == 0.)
				_end = (i+1)%N;
			if(input(i%N) == 0. && input((i+1)%N) > 0.)
				_start = (i+1)%N;
		}

		int factor = 1;

		double sins = 0.;
		double coss = 0.;


		uword index;
		double maxmax = input.max(index);

		if(_start > _end)
			_end+=N;


		for(int i = _start; i < _end; i++){
			//cout << i << " " << i%N << endl;
			output += (2*M_PI*i/N)*input(i%N);
			sum_act += input(i%N);
//			if(input(0) != 0.0){
//				continue;
//			}
//			else{
//				offset_i = i;
//				offset = preferred_angle(i);
//				break;
//			}
		}
		output /= sum_act;

		if(output > 0.)
			avg_angle = Angle(fmod(output, 2*M_PI));
		else{
			//printf("%f\n", output);
			avg_angle = Angle(2*M_PI+fmod(output, 2*M_PI));
		}


//		for(int i = 0; i < N; i++){
//			output += preferred_angle((i+offset_i)%N)*input((i+offset_i)%N);
//			sum_act += input((i+offset_i)%N);
//		}
//		output /= sum_act;
//
//		if(output+offset > 0.)
//			avg_angle = Angle(fmod(output+offset, 2*M_PI));
//		else{
//			printf("%f\n", output+offset);
//			avg_angle = Angle(2*M_PI+fmod(output+offset, 2*M_PI));
//		}
//		if(input(0) == 0.0){
//			for(int i = 0; i < N; i++){
//				if(input(i) > 0.0){
//					output += preferred_angle(i)*input(i);
//					sum_act += input(i);
//				}
//			}
//			if(sum_act>0.0)
//				output /= sum_act;
//		}
//		else{
//			for(int i = 0; i < N; i++){
//				if(input(i) > 0.0){
//					output += preferred_angle(i)*input(i);
//					sum_act += input(i);
//				}
//			}
//			if(sum_act>0.0)
//				output /= sum_act;
//		}
	};

	/**
	 * Updates the length of array rate
	 *
	 * @param (vec) input: vector
	 * @return (void)
	 */
	void update_len(vec input){
		length = scale_factor * sum(input)/(N*N);
	};

	/**
	 * Updates the angle & rate of maximum firing neuron
	 *
	 * @return (void)
	 */
	void update_max(vec input){
		uword index;
		max_rate = input.max(index);
		max_angle = Angle(preferred_angle(index));
	};

	/**
	 * Updates the activities of the arrays
	 *
	 * @param (vec) input: vector
	 * @return (void)
	 */
	void update_rate(vec rate){
		if(rate.n_elem != output_rate.n_elem)
			printf("WARNING: Dimension has been changed.\n");
		output_rate = rate;
//		if(type == 0){
//			update_avg(output_rate);
//			update_len(output_rate);
//			update_max(output_rate);
//		}
//		else{
//			update_avg(input_conns.col(0));
//			update_len(input_conns.col(0));
//			update_max(input_conns.col(0));
//		}
	};

	/**
	 * Returns vector of Gaussian noise with given width
	 *
	 *  @param (int) dim: vector dimension
	 *  @param (double) width: standarad deviation of the normal distribution
	 *  @return (vec)
	 */
	vec vnoise(int dim, double width){
		if(width > 0.0){
			return width*randn<vec>(dim);
		}
		else
			return zeros<vec>(dim);
	};

	/**
	 * Returns the weight matrix of the array
	 *
	 *  @return (mat)
	 */
	mat w(){
		return input_conns;
	};

	int type;                                       // 0 = rate, 1 = weight

protected:

	int N;                                          // Number of neurons
	int K;                                          // Input dimension
	vec output_rate;								// Activity rate of neuron array
	mat input_conns;                                // incoming connections
	vec preferred_angle;                            // Preferred angle of neurons

private:

	Angle max_angle;                                // Angle of the maximum-firing neuron
	double max_rate;                                // Maximum rate of neuron array
	Angle avg_angle;                                // Average position of the maximum firing rate
	double length;                                  // Length of vector = (some scaling factor)*(sum of activities)/N
	const double scale_factor = 2.41456;			// Scaling factor (1.25597(fit_mult); 2.41456(fit_add); 2.41474212(manual_add))

	vec input_rate;                                 // Input activity rate to the array
	vec bias;										// Bias vector
	unsigned int seed;
};


#endif /* CIRCULARARRAY_H_ */
