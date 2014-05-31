/*
 * Flow.cpp
 *
 *  Created on: 2014-05-29
 *      Author: jtestard
 */


#include "Flow.h"

const string Flow::TOP;
const string Flow::BOTTOM;

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

//Most basic join operation possible.
Flow Flow::join(Flow &other){
	//join bottom-bottom gives you bottom. Anything else gives you top.
	if (this->basic==BOTTOM && other.basic==BOTTOM)
		return Flow(BOTTOM);
	else
		return Flow(TOP);
}

Flow::~Flow(){
	//Nothing for basic static analysis
}
