/*
 * geom.h
 *
 *  Created on: 31.07.2015
 *      Author: meicron
 */

#ifndef GEOM_H_
#define GEOM_H_

#include <iostream>
#include <cmath>
#include <cstdio>
#include <vector>
using namespace std;

/**
 * Angle Class
 *
 * 	This class creates angles and handles angle-related functions
 *
 */

enum aunit{inRad,inDeg,inGon};

class Angle {
public:
	Angle()
	{
		val.resize(3);
		vmax.resize(3);
		vmin.resize(3);

		vmin.at(inRad) = 0.;
		vmax.at(inRad) = 2.*M_PI;
		vmin.at(inDeg) = 0.;
		vmax.at(inDeg) = 360.;
		vmin.at(inGon) = 0.;
		vmax.at(inGon) = 200.;


		rad_ = &val.at(inRad);
		deg_ = &val.at(inDeg);
		gon_ = &val.at(inGon);

		for(int x=inRad; x<=inDeg; x++)
			val.at(x) = 0.;
	}
	Angle(double _val)
	{
		val.resize(3);
		vmax.resize(3);
		vmin.resize(3);

		vmin.at(inRad) = 0.;
		vmax.at(inRad) = 2.*M_PI;
		vmin.at(inDeg) = 0.;
		vmax.at(inDeg) = 360.;
		vmin.at(inGon) = 0.;
		vmax.at(inGon) = 200.;

		rad_ = &val.at(inRad);
		deg_ = &val.at(inDeg);
		gon_ = &val.at(inGon);

		val.at(inRad) = fmod(_val,(vmax.at(inRad)));
		convertTo(val.at(inRad), inRad);
	}
	Angle(double _val, int _unit)
	{
		val.resize(3);
		vmax.resize(3);
		vmin.resize(3);

		vmin.at(inRad) = 0.;
		vmax.at(inRad) = 2.*M_PI;
		vmin.at(inDeg) = 0.;
		vmax.at(inDeg) = 360.;
		vmin.at(inGon) = 0.;
		vmax.at(inGon) = 400.;

		rad_ = &val.at(inRad);
		deg_ = &val.at(inDeg);
		gon_ = &val.at(inGon);

		val.at(_unit) = fmod(_val,(vmax.at(_unit)));
		convertTo(val.at(_unit), _unit);
	}

	void convertTo(double _val, int _unit){
		if(_unit == inRad){
			val.at(inDeg) = _val*180./M_PI;
			val.at(inGon) = _val*200./M_PI;
		}
		if(_unit == inDeg){
			val.at(inRad) = _val*M_PI/180.;
			val.at(inGon) = _val*200./180.;
		}
		if(_unit == inGon){
			val.at(inRad) = _val*M_PI/200.;
			val.at(inDeg) = _val*180./200.;

		}
	}

	double fmod(double _x, double _y){
		if(_x > 0.)
			return std::fmod(_x,_y);
		else
			return _y + std::fmod(_x,_y);
	}

	friend Angle operator+(Angle lhs, Angle rhs)
	{
		double number = lhs.rad() + rhs.rad();
		Angle ra(number);
		return ra;
	}
	friend Angle operator-(Angle lhs, Angle rhs)
	{
		double number = lhs.rad() - rhs.rad();
		Angle ra(number);
		return ra;
	}
	Angle operator*(double f)
	{
		double number = rad()*f;
		Angle ra(number);
		return ra;
	}
	Angle operator/(double f)
	{
		double number = rad()/f;
		Angle ra(number);
		return ra;
	}
	friend ostream& operator<<(ostream& out, const Angle& a) // output
	{
	    out << *(a.deg_) <<  "°";
	    return out;
	}

	void setTo(double _val, int _unit = inRad){
		val.at(_unit) = _val;
		if(_unit == inRad){
			val.at(inDeg) = _val*180./M_PI;
			val.at(inGon) = _val*200./M_PI;
		}
		if(_unit == inDeg){
			val.at(inRad) = _val*M_PI/180.;
			val.at(inGon) = _val*200./180.;
		}
		if(_unit == inGon){
			val.at(inRad) = _val*M_PI/200.;
			val.at(inDeg) = _val*180./200.;
		}
	}

	double Sin(){return sin(rad());}
	double Cos(){return cos(rad());}
	double S(){return sin(rad());}
	double C(){return cos(rad());}


	double deg(){
		return *deg_;
	}

	double gon(){
		return *gon_;
	}

	double rad(){
		return *rad_;
	}

private:
	double* deg_;
	double* gon_;
	double* rad_;
	vector<double> val;
	vector<double> vmax;
	vector<double> vmin;
};

/**
 * 3D Vector Class
 *
 * 	This class creates 3D vectors and handles vector-related functions
 *
 */

class Vec {
public:
	Vec(){ x=y=z=0.; array[0]=array[1]=array[2]=0.;}
	Vec(double x_, double y_){	x=x_; y=y_; z=0.; array[0]=array[1]=array[2]=0.;}
	Vec(double x_, double y_, double z_){	x=x_; y=y_; z=z_; array[0]=array[1]=array[2]=0.;}
	~Vec(){}

	Angle azimuth() { return Angle(atan2(y,x)); }
	Angle ang(){ return azimuth();}
	Angle elevation() { return Angle(atan2(z, len())); }
	double len() { return sqrt(x*x+y*y+z*z); }

	Vec operator+(const Vec& sum) const
	{ Vec rv(x+sum.x, y+sum.y, z+sum.z); return rv; }
	Vec operator-(const Vec& sum) const
	{ Vec rv(x-sum.x, y-sum.y, z-sum.z); return rv; }
	Vec operator*(double f) const { Vec rv(x*f, y*f, z*f); return rv; }
	friend ostream& operator<<(ostream& out, const Vec& vec) // output
	{
	    out << "(" << vec.x << ", " << vec.y << ", " << vec.z <<  ")";
	    return out;
	}

    void print() const { printf("(%g, %g, %g)\n", x, y, z); }
    void set(double x_, double y_, double z_=0) { x=x_; y=y_; z=z_; }
    const double* toArray(){ array[0]=x;array[1]=y; array[2]=z; return array; }

	double x;
	double y;
	double z;

private:
  double array[3];
};



#endif /* GEOM_H_ */
