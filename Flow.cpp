/*
 * Flow.cpp
 *
 *  Created on: 2014-05-29
 *      Author: jtestard
 */


#include "Flow.h"

/**
 * For the basic static analysis, we assume all flows are equal.
 */
bool Flow::operator==(const Flow& other) const{
	return true;
}

string Flow::jsonString(){
	return "\"" + basic + "\"";
}

Flow & Flow::operator=(const Flow &rhs){
	basic = rhs.basic;
	return *this;
}

Flow::Flow(){
	basic = "";
}

Flow::Flow(string input){
	basic = input;
}


Flow::~Flow(){
	//Nothing for basic static analysis
}
