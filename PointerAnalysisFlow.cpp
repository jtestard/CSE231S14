/*

 * Flow.cpp
 *
 *  Created on: 2014-05-29
 *      Author: jtestard
 */

#include "PointerAnalysisFlow.h"

/*
 * Flows are equal if their values are equal
 */
bool PointerAnalysisFlow::equals(Flow* otherSuper) {
	PointerAnalysisFlow* other = static_cast<PointerAnalysisFlow*>(otherSuper);
	if (other->value.size()!=this->value.size())
			return false;
	for (map<string, set<string> >::const_iterator it = this->value.begin(); it != this->value.end() ; it++) {
		string key = it->first;
		set<string> thisSet = it->second;
		//Check if key is found in other
		if(other->value.find(key)==other->value.end())
			return false;
		set<string> otherSet = other->value.find(key)->second;
		 for (set<string>::iterator it=thisSet.begin(); it!=thisSet.end(); ++it) {
			 if(otherSet.find(*it)==otherSet.end()){
				 return false;
			 }
		 }
	}
	return true;
}

string PointerAnalysisFlow::jsonString() {
	if (basic!="")
		return "\"" + basic + "\"";
	stringstream ss;
	map<string, set<string> >::const_iterator it = this->value.begin();
	ss << "\"" << it->first << "\" : [ ";
	set<string>::iterator its=it->second.begin();
	ss << *its << " ";
	for (; its != it->second.end() ; its++) {
		ss << ", " << *its;
	}
	ss << " ] ";
	for (; it != this->value.end() ; it++) {
		ss << "\"" << it->first << "\" : [ ";
		its=it->second.begin();
		ss << *its << " ";
		for (; its != it->second.end() ; its++) {
			ss << ", " << *its;
		}
		ss << " ] ";
	}
	return ss.str();
}

void PointerAnalysisFlow::copy(Flow* rhs) {
	PointerAnalysisFlow* f = static_cast<PointerAnalysisFlow*>(rhs);
	this->basic = f->basic;
	this->value = f->value;
}

PointerAnalysisFlow::PointerAnalysisFlow() :
		Flow() {
}

PointerAnalysisFlow::PointerAnalysisFlow(string input) :
		Flow(input) {
}

PointerAnalysisFlow::PointerAnalysisFlow(Flow *flow) :
		Flow(flow->basic) {
}

//Merges flow together.
Flow* PointerAnalysisFlow::join(Flow* otherSuper) {
	//errs()<< "PointerAnalysisFlow::join called...\n";
	//join bottom-bottom gives you bottom. Anything else gives you top.
	PointerAnalysisFlow* other = static_cast<PointerAnalysisFlow*>(otherSuper);

	if (this->basic == BOTTOM && other->basic == BOTTOM)
		return new PointerAnalysisFlow(BOTTOM);

	//Anything joined with a bottom will just be itself.
	if (this->basic == BOTTOM)
		return other;
	if (other->basic == BOTTOM)
		return this;

	//Join anything with top will give you top.
	if (this->basic == TOP || other->basic == TOP)
		return new PointerAnalysisFlow(TOP);

	//Merge the input from both.
	PointerAnalysisFlow* f;
	//Merges this value with f
	for (map<string, set<string> >::const_iterator itThis = this->value.begin();
			itThis != this->value.end(); itThis++) {
		string varThis = itThis->first;
		set < string > setThis = itThis->second;
		//This lines uses the copy constructor of the STL set class.
		f->value[varThis].insert(setThis.begin(),setThis.end());
	}
	//merges other value with f
	for (map<string, set<string> >::const_iterator itOther =
			other->value.begin(); itOther != other->value.end(); itOther++) {
		string varOther = itOther->first;
		set < string > setOther = itOther->second;
		f->value[varOther].insert(setOther.begin(),setOther.end());

	}
	return f;

}

PointerAnalysisFlow::~PointerAnalysisFlow() {
	//Nothing for basic static analysis
}
