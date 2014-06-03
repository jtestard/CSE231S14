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
	ss << "{\"" << it->first << "\" : [ ";
	set<string>::iterator its=it->second.begin();
	if (its != it->second.end()) {
	ss << *its << " "; its++;
	}
	for (; its != it->second.end() ; its++) {
		ss << ", " << *its;
	}
	//errs() << "number of keys in set : " << it->second.size() << "\n";
 	ss << " ] ";
 	if (it != this->value.end())
 		it++;
	for (; it != this->value.end() ; it++) {
		if (it->second.size()==0)
			continue;
		ss << ", \"" << it->first << "\" : [ ";
		its=it->second.begin();
		if (its != it->second.end()) {
			ss << *its << " ";
			its++;
		}
		for (; its != it->second.end() ; its++) {
			ss << ", " << *its;
		}
		ss << "] ";
	}
	ss << "}";
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
	this->value = flow->value;
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

	//Merge the input from both.
	PointerAnalysisFlow* f = new PointerAnalysisFlow();

	//Get all keys
	set<string> keys;
	for (map<string, set<string> >::iterator it = this->value.begin() ; it != this->value.end() ; it++)
		keys.insert(it->first);
	for (map<string, set<string> >::iterator it = other->value.begin() ; it != other->value.end() ; it++)
		keys.insert(it->first);
	for (set<string>::iterator it = keys.begin() ; it != keys.end() ; it++) {
		string key = *it;
		set<string> values;
		for (set<string>::iterator j = this->value[key].begin(); j != this->value[key].end() ; j++)
			values.insert(*j);
		for (set<string>::iterator j = other->value[key].begin(); j != other->value[key].end() ; j++)
			values.insert(*j);
		if (values.size()>0)
			f->value[key] = values;
	}
	return f;

}

PointerAnalysisFlow::~PointerAnalysisFlow() {
	//Nothing for basic static analysis
}
