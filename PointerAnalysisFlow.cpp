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
bool PointerAnalysisFlow::equals(Flow& other) {
//	PointerAnalysisFlow* other = (PointerAnalysisFlow*)otherSuper;
//	if (other->value.size()!=this->value.size())
//		return false;
//	for (map<string, vector<string> >::const_iterator itThis = this->value.begin(), itOther = other.value.begin() ;
//			itThis != this->value.end() ; itThis++, itOther++) {
//			string varThis = itThis->first;
//			string varOther = itOther->first;
//			vector<string> vecThis = itThis->second;
//			vector<string> vecOther = itOther->second;
//			if (vecThis.size()!=vecOther.size())
//				return false;
//			for (int i = 0 ; i < vecThis.size() ; i++) {
//				if (vecThis[i]!=vecOther[i])
//					return false;
//			}
//	}
	return true;
}

string PointerAnalysisFlow::jsonString(){
	return "\"" + basic + "\"";
}

void PointerAnalysisFlow::copy(Flow &rhs){
//	this->basic = ((PointerAnalysisFlow)rhs).basic;
//	this->value = ((PointerAnalysisFlow)rhs).value;
}

PointerAnalysisFlow::PointerAnalysisFlow() : Flow() { }

PointerAnalysisFlow::PointerAnalysisFlow(string input) : Flow(input){}

//Most basic join operation possible.
Flow PointerAnalysisFlow::join(Flow &other){
	//join bottom-bottom gives you bottom. Anything else gives you top.
	if (this->basic==BOTTOM && other.basic==BOTTOM)
		return Flow(BOTTOM);
	else
		return Flow(TOP);
}

PointerAnalysisFlow::~PointerAnalysisFlow(){
	//Nothing for basic static analysis
}
