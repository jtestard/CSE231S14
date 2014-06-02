/*

 * Flow.cpp
 *
 *  Created on: 2014-05-29
 *      Author: jtestard
 */

#include "ConstantPropAnalysisFlow.h"

/*
 * Flows are equal if their values are equal
 */
bool ConstantPropAnalysisFlow::equals(Flow* otherSuper) {
	ConstantPropAnalysisFlow* other = static_cast<ConstantPropAnalysisFlow*>(otherSuper);
	if (other->value.size()!=this->value.size())
			return false;
	for (map<string, float >::const_iterator it = this->value.begin(); it != this->value.end() ; it++) {
		string key = it->first;
		float thisVal = it->second;
		//Check if key is found in other
		if(other->value.find(key)==other->value.end())
			return false;
		float otherVal = other->value.find(key)->second;
		if (otherVal!=thisVal)
			return false;

	}


	/*

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

	*/
	return true;
}

//Represents a constant propagation analysis value as a JSON string.
string ConstantPropAnalysisFlow::jsonString() {
	if (value.size()==0)
		return "\"" + basic + "\"";
	//Value has something inside
	stringstream ss;
	map<string, float >::const_iterator it = this->value.begin();
	/*ss << "{\"" << it->first << "\" : [ ";
	set<string>::iterator its=it->second.begin();
	ss << *its << " "; its++;
	for (; its != it->second.end() ; its++) {
		ss << ", " << *its;
	}
	ss << " ] ";
	*/
	int counter = 0;
	for (; it != this->value.end() ; it++) {
		if (counter==0){
			ss << "\"" << it->first << "\" : ";
			float v =it->second;
			ss << v << " ";
		}
		else{
			ss << ",\"" << it->first << "\" : ";
			float v =it->second;
			ss << v << " ";
		}

		counter++;

	}
	ss << "}";
	return ss.str();

	/*
	 *
	stringstream ss;
	map<string, set<string> >::const_iterator it = this->value.begin();
	ss << "{\"" << it->first << "\" : [ ";
	set<string>::iterator its=it->second.begin();
	ss << *its << " "; its++;
	for (; its != it->second.end() ; its++) {
		ss << ", " << *its;
	}
	ss << " ] ";
	for (; it != this->value.end() ; it++) {
		ss << "\"" << it->first << "\" : [ ";
		its=it->second.begin();
		ss << *its << " "; its++;
		for (; its != it->second.end() ; its++) {
			ss << ", " << *its;
		}
		ss << "] ";
	}
	ss << "}";
	return ss.str();
	 *
	 *
	 *
	 */
}

void ConstantPropAnalysisFlow::copy(Flow* rhs) {
	ConstantPropAnalysisFlow* f = static_cast<ConstantPropAnalysisFlow*>(rhs);
	this->basic = f->basic;
	this->value = f->value;
}

ConstantPropAnalysisFlow::ConstantPropAnalysisFlow() :
		Flow() {
}

ConstantPropAnalysisFlow::ConstantPropAnalysisFlow(string input) :
		Flow(input) {
}

ConstantPropAnalysisFlow::ConstantPropAnalysisFlow(ConstantPropAnalysisFlow *flow) :
		Flow(flow->basic) {
	this->value = flow->value;
}


// TODO : Fix the join... See the commented out stuff
//Merges flow together.
Flow* ConstantPropAnalysisFlow::join(Flow* otherSuper) {
	//join bottom-bottom gives you bottom. Anything else gives you top.
	ConstantPropAnalysisFlow* other = static_cast<ConstantPropAnalysisFlow*>(otherSuper);

	if (this->basic == BOTTOM && other->basic == BOTTOM)
		return new ConstantPropAnalysisFlow(BOTTOM);

	//Anything joined with a bottom will just be itself.
	if (this->basic == BOTTOM) {
		ConstantPropAnalysisFlow* f = new ConstantPropAnalysisFlow();
		f->copy(other);
		return f;
	}
	if (other->basic == BOTTOM) {
		ConstantPropAnalysisFlow* f = new ConstantPropAnalysisFlow();
		f->copy(this);
		return f;
	}

	//Join anything with top will give you top.
	if (this->basic == TOP || other->basic == TOP)
		return new ConstantPropAnalysisFlow(TOP);

	//Merge the input from both.
	ConstantPropAnalysisFlow* f = new ConstantPropAnalysisFlow();

	//f = other;
	for (map<string, float >::iterator it = this->value.begin() ; it != this->value.end() ; it++){

		if (other->value.find(it->first) == other->value.end() ) {
			// They don't have the same key! We're good!
			f->value[it->first] = this->value.find(it->first)->second;
		} else {
			// Oh no! They do have the same key! We need to check if they have
			// the same values! if they do then we're good
			float otherVal = other->value.find(it->first)->second;
			float thisVal = this->value.find(it->first)->second;

			if (otherVal == thisVal)
				// OK, we can replicate the value since both branches
				// had the same value for this variable
				f->value[it->first] = otherVal;

			//else
				// Nope! They have different values
				// we need to omit this variable for
				// the (implicit) "set"


		}
	}

	for (map<string, float >::iterator it = other->value.begin() ; it != other->value.end() ; it++){

			if (this->value.find(it->first) == this->value.end() ) {
				// They don't have the same key! We're good!
				f->value[it->first] = other->value.find(it->first)->second;
			} else {
				// Oh no! They do have the same key! We need to check if they have
				// the same values! if they do then we're good
				float thisVal = this->value.find(it->first)->second;
				float otherVal = other->value.find(it->first)->second;

				if (otherVal == thisVal)
					// OK, we can replicate the value since both branches
					// had the same value for this variable
					f->value[it->first] = otherVal;

				//else
					// Nope! They have different values
					// we need to omit this variable for
					// the (implicit) "set"


			}
		}
	//Get all keys
	/*set<string> keys;
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
		f->value[key] = values;
	}
	*/
	return f;

}

ConstantPropAnalysisFlow::~ConstantPropAnalysisFlow() {
	//Nothing for basic static analysis
}
