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

//TO DO: CHECK!!!!
bool RangeFlowSet::equals(Flow* otherSuper)
{
	RangeFlowSet* in = static_cast<RangeFlowSet*>(otherSuper);
	map<string, RangeDomainElement >::const_iterator myFlowSetItr, inFlowSetItr;

	if (in->value.size()!=this->value.size())
			return false;
	for (myFlowSetItr = this->value.begin(); myFlowSetItr != this->value.end() ; myFlowSetItr++)
	{
		string varName = myFlowSetItr->first;	//Get the variable's name
//		RangeDomainElement varRange = myFlowSetItr->second;
		//Check if the first variable name is found in the input set
		inFlowSetItr = in->value.find(varName);
		if(inFlowSetItr == in->value.end())
			return false;	//Not found, exit.

		//Check that the Flow set elements have the same value
		if(!RangeDomainElementisEqual(&myFlowSetItr->second,&inFlowSetItr->second))
			return false;
		//The variable exists, so now check if the variable's range is correct
		/*set<string> otherSet = other->value.find(key)->second;
		 for (set<string>::iterator it=thisSet.begin(); it!=thisSet.end(); ++it) {
			 if(otherSet.find(*it)==otherSet.end()){
				 return false;
			 }
		 }*/
	}
	return true;
}

//Check if the value set of this RangeFlowSet contains a variable.
bool RangeFlowSet::contains(string &varName)
{
	if(this->value.find(varName) != this->value.end())
		return true;

	return false;
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
	basic = Flow::BOTTOM;
}

RangeFlowSet::RangeFlowSet(string input) :
		Flow(input) {
	basic = input;
}

//Not required....
/*
RangeFlowSet::RangeFlowSet(RangeFlowSet *flow) :
		Flow(flow->basic)
{
		//this->value = *flow;
}
*/

//Merges flow together.
//MOST DIFFICULT PART
Flow* RangeFlowSet::join(Flow* otherSuper) {
	//join bottom-bottom gives you bottom. Anything else gives you top.
	RangeFlowSet* in = static_cast<RangeFlowSet*>(otherSuper);
	RangeFlowSet* AUB = new RangeFlowSet();						//The join set of A and B. A is "this" B is "in"
	map<string, RangeDomainElement >::const_iterator myFlowSetItr, inFlowSetItr;


	if (this->basic == BOTTOM && in->basic == BOTTOM)
		return new RangeFlowSet(BOTTOM);

	//Anything joined with a bottom will just be itself.
	if (this->basic == BOTTOM) {
		AUB->copy(in);
		return AUB;
	}
	if (in->basic == BOTTOM) {
		AUB->copy(this);
		return AUB;
	}

	//Join anything with top will give you top.
	if (this->basic == TOP || in->basic == TOP)
		return new RangeFlowSet(TOP);


	//Merge the input from both.


	//If you find a variable living in a "flow set" then you should merge with the largest range possible.
	//Go through all of the variables one by one in my set. If I have some he doesn't just add them as they are to the new thing.
	//Otherwise, get the least precise information and add that to the new thing
	for (myFlowSetItr = this->value.begin(); myFlowSetItr != this->value.end() ; myFlowSetItr++)
	{
		string varName = myFlowSetItr->first;	//Get the variable's name
		//Check if the first variable name is found in the input set
		inFlowSetItr = in->value.find(varName);
		if(inFlowSetItr != in->value.end())
		{
			AUB->value[varName] = JoinRangeDomainElements(&myFlowSetItr->second,&inFlowSetItr->second);
		}
		else
		{
			//Copy the range and insert it as the value for a copy of the variable in my set of variable ranges.
			RangeDomainElement copiedRange;
			copiedRange.upper = myFlowSetItr->second.upper;
			copiedRange.lower = myFlowSetItr->second.lower;
			AUB->value[varName] = copiedRange;
		}

	}
	//Go through all of the variables one by one in his set. If he has some I don't just add them as they are to the new thing.
	//Don't get the least precise version and add to the new thing, that was done already.
	for(inFlowSetItr = in->value.begin(); inFlowSetItr != in->value.end(); inFlowSetItr++)
	{
		string varName = inFlowSetItr->first;	//Get the variables name
		//Check if the variable name can be found in my set
		myFlowSetItr  = this->value.find(varName);
		if(myFlowSetItr == this->value.end())
		{
			//Copy the range and insert it as the value for a copy of the variable in my set of variable ranges.
			RangeDomainElement copiedRange;
			copiedRange.upper = inFlowSetItr->second.upper;
			copiedRange.lower = inFlowSetItr->second.lower;
			AUB->value[varName] = copiedRange;
		}
	}

	//Could be a more optimal way of doing that but I don't care. Make something work first.


	return AUB;

}

RangeFlowSet::~RangeFlowSet() {
	//Nothing for basic static analysis
}

/*---------------------------------------------------------------------------------*\
 * 																					*
 * 							UTILITY FUNCTIONS										*
 * 																					*
\*----------------------------------------------------------------------------------*/
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

	return maxAB;
}

/*---------------------------------------------------------------------------------*\
 * 																					*
 * 						END UTILITY FUNCTIONS										*
 * 																					*
\*----------------------------------------------------------------------------------*/

