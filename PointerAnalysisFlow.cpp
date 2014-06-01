/*
 * Flow.cpp
 *
 *  Created on: 2014-05-29
 *      Author: jtestard
 */

#include "PointerAnalysisFlow.h"

/**
 * Flows are equal if their values are equal
 */
bool PointerAnalysisFlow::equals(Flow& otherSuper) {
	PointerAnalysisFlow other = (PointerAnalysisFlow) otherSuper;
//	return (*this).value.size() == other.value.size()
//			&& equal((*this).value.begin(), (*this).value.end(),
//					other.value.begin());
//	if (other.value.size()!=this->value.size())
//		return false;
//	for (map<string, set<string> >::const_iterator itThis = this->value.begin(), itOther = other.value.begin() ;
//			itThis != this->value.end() ; itThis++, itOther++) {
//			string varThis = itThis->first;
//			string varOther = itOther->first;
//			set<string> setThis = itThis->second;
//			set<string> setOther = itOther->second;
//			if (varThis==varOther) {
//
//			}
//			if (setThis.size()!=setOther.size())
//				return false;
//			if (!(equal(set))
//	}
	return (*this).basic==other.basic;

}

string PointerAnalysisFlow::jsonString() {
	return "\"" + basic + "\"";
}

void PointerAnalysisFlow::copy(Flow &rhs) {
	PointerAnalysisFlow f = (PointerAnalysisFlow) rhs;
	this->basic = f.basic;
	this->value = f.value;
}

PointerAnalysisFlow::PointerAnalysisFlow() :
		Flow() {
}

PointerAnalysisFlow::PointerAnalysisFlow(string input) :
		Flow(input) {
}

PointerAnalysisFlow::PointerAnalysisFlow(Flow &flow) :
		Flow(flow.basic) {
}

//Merges flow together.
Flow PointerAnalysisFlow::join(Flow &otherSuper) {
	errs()<< "In PointerAnalysisFlow::join()\n";
	//join bottom-bottom gives you bottom. Anything else gives you top.
	PointerAnalysisFlow other = (PointerAnalysisFlow) otherSuper;

	if (this->basic == BOTTOM && other.basic == BOTTOM)
		return PointerAnalysisFlow(BOTTOM);

	//Anything joined with a bottom will just be itself.
	if (this->basic == BOTTOM)
		return (PointerAnalysisFlow) other;
	if (other.basic == BOTTOM)
		return *this;

	//Join anything with top will give you top.
	if (this->basic == TOP || other.basic == TOP)
		return PointerAnalysisFlow(TOP);

	//Merge the input from both.
	PointerAnalysisFlow f;
	for (map<string, set<string> >::const_iterator itThis = this->value.begin();
			itThis != this->value.end(); itThis++) {
		string varThis = itThis->first;
		set < string > setThis = itThis->second;
		//This lines uses the copy constructor of the STL set class.
		f.value[varThis].insert(setThis.begin(),setThis.end());
	}
	for (map<string, set<string> >::const_iterator itOther =
			other.value.begin(); itOther != other.value.end(); itOther++) {
		string varOther = itOther->first;
		set < string > setOther = itOther->second;
		f.value[varOther].insert(setOther.begin(),setOther.end());

	}
	return f;

}

PointerAnalysisFlow::~PointerAnalysisFlow() {
	//Nothing for basic static analysis
}
