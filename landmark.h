/*
 * landmark.h
 *
 *  Created on: Jul 17, 2014
 *      Author: degoldschmidt
 */

#ifndef LANDMARK_H_
#define LANDMARK_H_

using namespace std;

class Landmark {
public:
	Landmark(double max_radius);
	Landmark(double x, double y);
	~Landmark();

	double x_position;
	double y_position;
	double distance_to_origin;
	double angle_to_x_axis;

	double get_hit(double x, double y);
	double rand(double min, double max);
	int total_hits;
};



#endif /* LANDMARK_H_ */
