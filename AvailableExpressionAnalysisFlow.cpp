/*

 * Flow.cpp
 *
 *  Created by Costas Zarifis on 22/05/2014.
 */

#include "AvailableExpressionAnalysisFlow.h"

/*
 * Flows are equal if their values are equal
 */
bool AvailableExpressionAnalysisFlow::equals(Flow* otherSuper) {
	AvailableExpressionAnalysisFlow* other =
			static_cast<AvailableExpressionAnalysisFlow*>(otherSuper);
	if (this->isBasic() || other->isBasic())
		return this->basicEquals(other);
	if (other->value.size() != this->value.size())
		return false;
	for (map<string, string>::const_iterator it = this->value.begin();
			it != this->value.end(); it++) {
		string key = it->first;
		string thisVal = it->second;
		//Check if key is found in other
		if (other->value.find(key) == other->value.end())
			return false;
		string otherVal = other->value.find(key)->second;
		if (otherVal != thisVal)
			return false;

	}

	return true;
}

//Represents a constant propagation analysis value as a JSON string.
string AvailableExpressionAnalysisFlow::jsonString() {
	if (value.size() == 0)
		return "\"" + basic + "\"";
	//Value has something inside
	stringstream ss;
	map<string, string>::const_iterator it = this->value.begin();
	/*ss << "{\"" << it->first << "\" : [ ";
	 set<string>::iterator its=it->second.begin();
	 ss << *its << " "; its++;
	 for (; its != it->second.end() ; its++) {
	 ss << ", " << *its;
	 }
	 ss << " ] ";
	 */

	int counter = 0;
	for (; it != this->value.end(); it++) {
		if (counter == 0) {
			ss << "\"" << it->first << "\" : ";
			string v = it->second;
			ss << v << " ";
		} else {
			ss << ",\"" << it->first << "\" : ";
			string v = it->second;
			ss << v << " ";
		}

		counter++;

	}
	ss << "}";
//	errs() << "After jSonString()...\n";
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

void AvailableExpressionAnalysisFlow::copy(Flow* rhs) {
	AvailableExpressionAnalysisFlow* f = static_cast<AvailableExpressionAnalysisFlow*>(rhs);
	this->basic = f->basic;
	this->value = f->value;
}

AvailableExpressionAnalysisFlow::AvailableExpressionAnalysisFlow() :
		Flow() {
}

AvailableExpressionAnalysisFlow::AvailableExpressionAnalysisFlow(string input) :
		Flow(input) {
}

AvailableExpressionAnalysisFlow::AvailableExpressionAnalysisFlow(
		AvailableExpressionAnalysisFlow *flow) :
		Flow(flow->basic) {
	this->value = flow->value;
}

// TODO : Fix the join... See the commented out stuff
//Merges flow together.
Flow* AvailableExpressionAnalysisFlow::join(Flow* otherSuper) {
	//join bottom-bottom gives you bottom. Anything else gives you top.
	AvailableExpressionAnalysisFlow* other =
			static_cast<AvailableExpressionAnalysisFlow*>(otherSuper);
//	errs()<< "I just entered into the sublcassed join... \n";

	if (this->basic == BOTTOM && other->basic == BOTTOM)
		return new AvailableExpressionAnalysisFlow(BOTTOM);

	//Anything joined with a bottom will just be itself.
	if (this->basic == BOTTOM) {
		AvailableExpressionAnalysisFlow* f = new AvailableExpressionAnalysisFlow();
		f->copy(other);
		return f;
	}
	if (other->basic == BOTTOM) {
		AvailableExpressionAnalysisFlow* f = new AvailableExpressionAnalysisFlow();
		f->copy(this);
		return f;
	}

	//Join anything with top will give you top.
	if (this->basic == TOP || other->basic == TOP)
		return new AvailableExpressionAnalysisFlow(TOP);

	//Merge the input from both.
	AvailableExpressionAnalysisFlow* f = new AvailableExpressionAnalysisFlow();

	//f = other;
	for (map<string, string>::iterator it = this->value.begin();
			it != this->value.end(); it++) {

		if (other->value.find(it->first) == other->value.end()) {
			// They don't have the same key! We're good!
			f->value[it->first] = this->value.find(it->first)->second;
		} else {
			// Oh no! They do have the same key! We need to check if they have
			// the same values! if they do then we're good
			string otherVal = other->value.find(it->first)->second;
			string thisVal = this->value.find(it->first)->second;

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

	for (map<string, string>::iterator it = other->value.begin();
			it != other->value.end(); it++) {

		if (this->value.find(it->first) == this->value.end()) {
			// They don't have the same key! We're good!
			f->value[it->first] = other->value.find(it->first)->second;
		} else {
			// Oh no! They do have the same key! We need to check if they have
			// the same values! if they do then we're good
			string thisVal = this->value.find(it->first)->second;
			string otherVal = other->value.find(it->first)->second;

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

//	errs()<< "JOINING!\n";
//	for (map<string, float>::iterator it = f->value.begin();
//				it != f->value.end(); it++) {
//		errs()<< it->first << " -> " << it->second << "\n";
//	}

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

AvailableExpressionAnalysisFlow::~AvailableExpressionAnalysisFlow() {
	//Nothing for basic static analysis
}
