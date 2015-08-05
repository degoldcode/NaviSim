/*
 * test_noise.cpp
 *
 *  Created on: 04.08.2015
 *      Author: meicron
 */

#include <iostream>
#include <fstream>
#include "../src/timer.h"
#include "../src/circulararray.h"
using namespace std;

ofstream data;

int main(){
	Timer timer(true);
	data.open("data/randomnum.dat");
	CircArray test;

	for(int i = 0; i<100000; i++)
		data  << test.boost_noise(0.1) << endl; //<<  << "\t"test.noise(0.1)

	data.close();
	auto elapsed_secs_cl = timer.Elapsed();
	printf("%4.3f s. Done.\n", elapsed_secs_cl.count()/1000.);
}


