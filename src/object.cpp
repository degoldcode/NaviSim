/*****************************************************************************
 *  object.cpp                                                               *
 *                                                                           *
 *  Created on:   May 11, 2015                                               *
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

#include <random>
#include "object.h"
using namespace std;

Object::Object(double max_radius, double min_radius){
	double distance_to_origin = (max_radius-min_radius) * sqrt(rand(0.0, 1.0)) + min_radius;
	double angle_to_x_axis = 2 * M_PI * rand(0.0, 1.0);
	double x_pos = distance_to_origin * cos(angle_to_x_axis);
	double y_pos = distance_to_origin * sin(angle_to_x_axis);

	pos.set(x_pos, y_pos);
}

Object::Object(double x, double y, double z){
	pos.set(x, y, z);
}

Object::~Object(){

}

double Object::d(){
	return pos.len();
}

double Object::d(double x, double y, double z){
	double rxsqr = pow(x-pos.x, 2.);
	double rysqr = pow(y-pos.y, 2.);
	double rdist = sqrt(rxsqr+rysqr);
	return rdist;
}

void Object::move(double _dx, double _dy, double _dz){
	pos.x += _dx;
	pos.y += _dy;
	pos.z += _dz;
}

double Object::rand(double min, double max){
	static random_device e{};
	static uniform_real_distribution<double> d(min, max);
	return d(e);
}

Angle Object::th(){
	return pos.azimuth();
}

Angle Object::th(double _x, double _y){
	return (pos-Vec(_x,_y)).azimuth();
}

void Object::to(double _x, double _y, double _z){
	pos.x = _x;
	pos.y = _y;
	pos.z = _z;
}

Vec Object::v(){
	return pos;
}

double Object::x(){
	return pos.x;
}

void Object::x(double _x){
	pos.x = _x;
}

double Object::y(){
	return pos.y;
}

void Object::y(double _y){
	pos.y = _y;
}

double Object::z(){
	return pos.z;
}

void Object::z(double _z){
	pos.z = _z;
}


/// testing angles


//int main() {
//	Angle first(M_PI/2.);
//	Angle second(320, inDeg);
//	Angle third(200, inGon);
//
//	cout << "First angle: " << first.rad() << " rad, " << first.deg() << "°, " << first.gon() << " gon\n";
//	cout << "Second angle: " << second.rad() << " rad, " << second.deg() << "°, " << second.gon() << " gon\n";
//	cout << "Third angle: " << third.rad() << " rad, " << third.deg() << "°, " << third.gon() << " gon\n\n";
//
//	cout << first.deg() << "°" << " + " << second.deg() << "° = " << (first+second).deg() << endl;
//}

//int main() {
//	Vec first(1., 2., 3.);
//	Vec second(4., 5., 6.);
//
//	cout << "First vector: " << first << endl ;
//	cout << "Second vector: " << second << endl;
//
//	cout << first + second << endl;
//}
