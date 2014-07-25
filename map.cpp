/*
 * main.cpp
 *
 *  Created on: Jul 3, 2014
 *      Author: degoldschmidt
 */

#include <ctime>
#include <cmath>
#include <vector>
#include <cassert>
#include <random>
#include "map.h"
using namespace std;


Map::Map(double param)
{
	stream.open("./data/map.dat", ios_base::out);
	pasemann = true;
	a_1 = 0.;
	a_2 = 0.;
	o_1 = rand(0.,1.);
	o_2 = rand(0.,1.);
	w_11 = 0.;
	w_12 = 0.;
	w_21 = 0.;
	b_1 = 0.;
	b_2 = 0.;
	set_weight(pasemann, param);

	param_map = param;
}

Map::~Map()
{
	stream.close();
}

double Map::update_map()
{
	a_1 = update_node(b_1, w_11, w_12, o_1, o_2);
	a_2 = update_node(b_2, 0., w_21, o_2, o_1);
	o_1 = sigm(a_1);
	o_2 = sigm(a_2);
	out_old = out;
	out = 0.5 * (o_1 + o_2); //average output
	if(!no_write)
		stream << out_old << "\t" << out << endl;
	return out;
}

double Map::update_node(double bias, double self_con, double coup_con, double output, double input){
	return bias + self_con * output + coup_con * input;
}

double Map::sigm(double x){
	return 1./(1. + std::exp(-x));
}

void Map::set_weight(bool option, double param){
	if(option){
		w_11 = -20.;
		w_12 = 6.;
		w_21 = -6.;
		b_1 = -2.;
		b_2 = 3.;
	}
	else{
		w_11 = -22.;
		w_12 = 5.9;
		w_21 = -6.6;
		b_1 = -3.4;
		b_2 = 3.8;
	}
	w_11 = param;
}

void Map::reset(){
	a_1 = 0.;
	a_2 = 0.;
	o_1 = rand(0.,1.);
	o_2 = rand(0.,1.);
}

double Map::rand(double min, double max){
	static random_device e{};
	static uniform_real_distribution<double> d(min, max);
	return d(e);
}


