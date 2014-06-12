/*

 * Flow.cpp
 *
 *  Created on: 2014-05-29
 *      Author: jtestard
 */

#include "RangeAnalysisFlow.h"

/*
 * Flows are equal if their values are equal
 */
bool RangeAnalysisFlow::equals(Flow* otherSuper)
{
	RangeAnalysisFlow* other =
			static_cast<RangeAnalysisFlow*>(otherSuper);
	if (this->isBasic() || other->isBasic())
		return this->basicEquals(other);
	if (other->value.size() != this->value.size())
		return false;
	for (map<string, RangeDomainElement>::const_iterator it = this->value.begin();
			it != this->value.end(); it++) {
		string key = it->first;
		RangeDomainElement thisVal = it->second;
		//Check if key is found in other
		if (other->value.find(key) == other->value.end())
			return false;
		RangeDomainElement otherVal = other->value.find(key)->second;
		if(!RangeDomainElementisEqual(	(const RangeDomainElement*) &otherVal,
										(const RangeDomainElement*) &thisVal)	)
			return false;

	}

	return true;
}

//Represents a constant propagation analysis value as a JSON string.
string RangeAnalysisFlow::jsonString() {
	if (value.size() == 0)
		return "\"" + basic + "\"";
	//Value has something inside
	stringstream ss;
	map<string, RangeDomainElement>::const_iterator it = this->value.begin();
	int counter = 0;
	ss << "{";
	for (; it != this->value.end(); it++) {
		if (counter == 0) {
			ss << "\"" << it->first << "\" : [";
			RangeDomainElement v = it->second;
			ss << v.lower << "," << v.upper << "] ";
		} else {
			ss << ",\"" << it->first << "\" : [";
			RangeDomainElement v = it->second;
			ss << v.lower << "," << v.upper << "] ";
		}

		counter++;

	}
	ss << "}";
	return ss.str();
}

void RangeAnalysisFlow::copy(Flow* rhs) {
	RangeAnalysisFlow* f = static_cast<RangeAnalysisFlow*>(rhs);
	this->basic = f->basic;
	this->value = f->value;
}

RangeAnalysisFlow::RangeAnalysisFlow() :
		Flow() {
}

RangeAnalysisFlow::RangeAnalysisFlow(string input) :
		Flow(input) {
}

RangeAnalysisFlow::RangeAnalysisFlow(
		RangeAnalysisFlow *flow) :
		Flow(flow->basic) {
	this->value = flow->value;
}

// TODO : Fix the join... See the commented out stuff
//Merges flow together.
Flow* RangeAnalysisFlow::join(Flow* otherSuper) {
	//join bottom-bottom gives you bottom. Anything else gives you top.
	RangeAnalysisFlow* other =
			static_cast<RangeAnalysisFlow*>(otherSuper);
//	errs()<< "I just entered into the sublcassed join... \n";

	if (this->basic == BOTTOM && other->basic == BOTTOM)
		return new RangeAnalysisFlow(BOTTOM);

	//Anything joined with a bottom will just be itself.
	if (this->basic == BOTTOM) {
		RangeAnalysisFlow* f = new RangeAnalysisFlow();
		f->copy(other);
		return f;
	}
	if (other->basic == BOTTOM) {
		RangeAnalysisFlow* f = new RangeAnalysisFlow();
		f->copy(this);
		return f;
	}

	//Join anything with top will give you top.
	if (this->basic == TOP || other->basic == TOP)
		return new RangeAnalysisFlow(TOP);

	//Merge the input from both.
	RangeAnalysisFlow* f = new RangeAnalysisFlow();

	//f = other;
	for (map<string, RangeDomainElement>::iterator it = this->value.begin();
			it != this->value.end(); it++) {

		if (other->value.find(it->first) == other->value.end()) {
			// They don't have the same key! We're good!
			f->value[it->first] = this->value.find(it->first)->second;
		} else {
			// Oh no! They do have the same key! We need to check if they have
			// the same values! if they do then we're good
			RangeDomainElement otherVal = other->value.find(it->first)->second;
			RangeDomainElement thisVal = this->value.find(it->first)->second;

			//if (otherVal == thisVal)
			//take the largest range of the two values to be the new range
			f->value[it->first] = JoinRangeDomainElements(	(const RangeDomainElement*) &otherVal,
																			(const RangeDomainElement*) &thisVal);
			/*
			if(RangeDomainElementisEqual(	(const RangeDomainElement*) &otherVal,
											(const RangeDomainElement*) &thisVal)	)
				// OK, we can replicate the value since both branches
				// had the same value for this variable
				f->value[it->first] = otherVal;

			else
			{
				f->value[it->first] = JoinRangeDomainElements(	(const RangeDomainElement*) &otherVal,
																(const RangeDomainElement*) &thisVal);
			}

			*/
			// Nope! They have different values
			// we need to omit this variable for
			// the (implicit) "set"

		}
	}

	for (map<string, RangeDomainElement>::iterator it = other->value.begin();
			it != other->value.end(); it++) {

		if (this->value.find(it->first) == this->value.end()) {
			// They don't have the same key! We're good!
			f->value[it->first] = other->value.find(it->first)->second;
		} else {
			// Oh no! They do have the same key! We need to check if they have
			// the same values! if they do then we're good
			RangeDomainElement thisVal = this->value.find(it->first)->second;
			RangeDomainElement otherVal = other->value.find(it->first)->second;

			//if (otherVal == thisVal)
			if(RangeDomainElementisEqual(	(const RangeDomainElement*) &otherVal,
											(const RangeDomainElement*) &thisVal)	)
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

RangeAnalysisFlow::~RangeAnalysisFlow() {
	//Nothing for basic static analysis
}

//UTILITY FUNCTIONS
//TO DO: DEBUG THIS CODE
RangeDomainElement JoinRangeDomainElements(const RangeDomainElement* A, const RangeDomainElement* B)
{
	//This will be the largest range possible. When you join ranges, you take the least precise range possible
	//WARNING!!!! Requires that you dont have values out of range!!!!!
	//WARNING!!!!

	RangeDomainElement maxAB;

	//If A has the lowest of the low range, make maxAB take that value.
	maxAB.lower = (A->lower <= B->lower) ? A->lower : B->lower;
	//If A has the highest of the high range, make maxAB take that value.
	maxAB.upper = (A->upper >= B->upper) ? A->upper : B->upper;
	//Preserve the TOP if it has been set in one of these variables
	maxAB.top = (A->top | B->top);

	return maxAB;
}
//TO DO: DEBUG THIS CODE
bool RangeDomainElementisEqual(const RangeDomainElement* A, const RangeDomainElement* B)
{
	if(A->lower == B->lower)
	{
		if(A->upper == B->upper)
		{
			return true;
		}
	}
	return false;
}

//END UTILITY FUNCTIONS
