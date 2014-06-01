/*
 * Flow.cpp
 *
 *  Created on: 2014-05-29
 *      Author: jtestard
 */


#include "PointerAnalysisFlow.h"

/**
 * For the basic static analysis, just compare strings.
 */
bool PointerAnalysisFlow::operator==(const Flow& otherSuper) const{
	PointerAnalysisFlow* other = (PointerAnalysisFlow*)otherSuper;
	if (other->value.size()!=this->value.size())
		return false;
	for (map<string, vector<string> >::const_iterator itThis = this->value.begin(), itOther = other.value.begin() ;
			itThis != this->value.end() ; itThis++, itOther++) {
			string varThis = itThis->first;
			string varOther = itOther->first;
			vector<string> vecThis = itThis->second;
			vector<string> vecOther = itOther->second;
			if (vecThis.size()!=vecOther.size())
				return false;
			for (int i = 0 ; i < vecThis.size() ; i++) {
				if (vecThis[i]!=vecOther[i])
					return false;
			}
	}
	return true;
}

string PointerAnalysisFlow::jsonString(){
	return "\"" + basic + "\"";
}

Flow & PointerAnalysisFlow::operator=(const Flow &rhsSuper){
	PointerAnalysisFlow* rhs = (PointerAnalysisFlow*)rhsSuper;
	this->basic = rhs.basic;
	this->value = rhs.value;
	return *this;
}

PointerAnalysisFlow::PointerAnalysisFlow() : Flow() { }

PointerAnalysisFlow::PointerAnalysisFlow(string input) : Flow(input){}

//Most basic join operation possible.
PointerAnalysisFlow PointerAnalysisFlow::join(PointerAnalysisFlow &other){
	//join bottom-bottom gives you bottom. Anything else gives you top.
	if (this->basic==BOTTOM && other.basic==BOTTOM)
		return Flow(BOTTOM);
	else
		return Flow(TOP);
}

PointerAnalysisFlow::~PointerAnalysisFlow(){
	//Nothing for basic static analysis
}
