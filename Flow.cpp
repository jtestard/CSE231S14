/*
 * Flow.cpp
 *
 *  Created on: 2014-05-29
 *      Author: jtestard
 */


#include "Flow.h"

/**
 * For the basic static analysis, just compare strings.
 */
bool Flow::operator==(const Flow& other) const{
	return basic==other.basic;
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
