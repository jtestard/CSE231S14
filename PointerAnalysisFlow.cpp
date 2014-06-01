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

//Represents a pointer analysis value as a JSON string.
string PointerAnalysisFlow::jsonString() {
	if (value.size()==0)
		return "\"" + basic + "\"";
	//Value has something inside
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

PointerAnalysisFlow::PointerAnalysisFlow(PointerAnalysisFlow *flow) :
		Flow(flow->basic) {
}

//Merges flow together.
Flow* PointerAnalysisFlow::join(Flow* otherSuper) {
	//join bottom-bottom gives you bottom. Anything else gives you top.
	PointerAnalysisFlow* other = static_cast<PointerAnalysisFlow*>(otherSuper);

	if (this->basic == BOTTOM && other->basic == BOTTOM)
		return new PointerAnalysisFlow(BOTTOM);

	//Anything joined with a bottom will just be itself.
	if (this->basic == BOTTOM) {
		PointerAnalysisFlow* f = new PointerAnalysisFlow();
		f->copy(other);
		return f;
	}
	if (other->basic == BOTTOM) {
		PointerAnalysisFlow* f = new PointerAnalysisFlow();
		f->copy(this);
		return f;
	}

	//Join anything with top will give you top.
	if (this->basic == TOP || other->basic == TOP)
		return new PointerAnalysisFlow(TOP);

	errs() << "No top No bottom : " << this->value.size() << "," << other->value.size() << "\n";
	//Merge the input from both.
	PointerAnalysisFlow* f = new PointerAnalysisFlow();
	//Merges this value with f
	for (map<string, set<string> >::const_iterator itThis = this->value.begin();
			itThis != this->value.end(); itThis++) {
		string varThis = itThis->first;
		set < string > setThis = itThis->second;
		//This lines uses the copy constructor of the STL set class.
		f->value[varThis].insert(setThis.begin(),setThis.end());
	}
	errs() << "this done! other keys : ";
	//merges other value with f
	for (map<string, set<string> >::const_iterator itOther =
			other->value.begin(); itOther != other->value.end(); itOther++) {
		string varOther = itOther->first;
		errs() << varOther << ",";
//		set < string > setOther = itOther->second;
//		errs() << "set size : " << setOther.size() << "\n";
//		f->value[varOther].insert(setOther.begin(),setOther.end());

	}
	errs()<< "\njoin done!\n";
	return f;

}

PointerAnalysisFlow::~PointerAnalysisFlow() {
	//Nothing for basic static analysis
}
