/*
 * Flow.cpp
 *
 *  Created on: 2014-05-29
 *      Author: jtestard
 */


#include "RangeFlowSet.h"

using namespace std;
using namespace llvm;
/*
 * Flows are equal if their values are equal
 */
bool RangeFlowSet::equals(Flow* otherSuper) {
/*	RangeFlowSet* other = static_cast<RangeFlowSet*>(otherSuper);
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
	}*/
	return true;
}

//Represents a pointer analysis value as a JSON string.
string RangeFlowSet::jsonString() {
	if (value.size()==0)
		return "\"" + basic + "\"";
	//Value has something inside
//	stringstream ss;
	/*map<string, set<string> >::const_iterator it = this->value.begin();
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
	}*/
	return "FUCKu";//ss.str();
}

void RangeFlowSet::copy(Flow* rhs) {
	RangeFlowSet* f = static_cast<RangeFlowSet*>(rhs);
	this->basic = f->basic;
	this->value = f->value;
}

RangeFlowSet::RangeFlowSet() :
		Flow() {
}

RangeFlowSet::RangeFlowSet(string input) :
		Flow(input) {
}

RangeFlowSet::RangeFlowSet(RangeFlowSet *flow) :
		Flow(flow->basic) {
}

//Merges flow together.
Flow* RangeFlowSet::join(Flow* otherSuper) {
	//join bottom-bottom gives you bottom. Anything else gives you top.
	RangeFlowSet* other = static_cast<RangeFlowSet*>(otherSuper);

	if (this->basic == BOTTOM && other->basic == BOTTOM)
		return new RangeFlowSet(BOTTOM);

	//Anything joined with a bottom will just be itself.
	if (this->basic == BOTTOM) {
		RangeFlowSet* f = new RangeFlowSet();
		f->copy(other);
		return f;
	}
	if (other->basic == BOTTOM) {
		RangeFlowSet* f = new RangeFlowSet();
		f->copy(this);
		return f;
	}

	//Join anything with top will give you top.
	if (this->basic == TOP || other->basic == TOP)
		return new RangeFlowSet(TOP);


	//Merge the input from both.
	RangeFlowSet* f = new RangeFlowSet();
	//Merges this value with f
	/*for (map<string, set<string> >::const_iterator itThis = this->value.begin();
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

	}*/
	return f;

}

RangeFlowSet::~RangeFlowSet() {
	//Nothing for basic static analysis
}
