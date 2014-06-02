/*
 * Flow.h
 *
 *  Created on: 2014-05-29
 *      Author: jtestard
 */

#ifndef RANGEFLOW_H_
#define RANGEFLOW_H_
#include "Flow.h"
#include <limits>		//Has max int and min int.
#include <map>			//required to map each variable to a range
#include <set>			//required to hold each discovered variable, for analysis
#include <algorithm>	//lets you do BFS, DFS...
#include <string>

using namespace std;
using namespace llvm;
/**
 * This class is used to represent the information computed by static analyses.
 * It is a very general class but must overload the functions shown below.
 * It can be anything, but it should be most of the time have an attribute to map
 * each variable to the information computed by the analysis.
 *
 * This class is the closest we can get to a lattice definition.
 */

//this type represents the range for each variable.
typedef struct _RangeDomainElement{
	int upper;
	int lower;
	bool top;
	bool bottom;
	_RangeDomainElement()
	{
			upper = std::numeric_limits<int>::max();
			lower = std::numeric_limits<int>::min();
			top = false;
			bottom = true;
	}
}RangeDomainElement;

class RangeFlowSet: public Flow {

public :
//	static const string TOP;
//	static const string BOTTOM;


	//The equality operator is used by the worklist algorithm and must be overloaded by the analysis.
	bool equals(Flow* other);

	/*
	 * This method is used by the JSONCFG function of the analysis to output the graph in JSON format.
	 * It must output a proper representation of the flow in JSON format. For example, for constant propagation :
	 *
	 * 		{ "X" : 20,	"Y" : 35, "Z" : 2.23, "W" : "d"	}
	 *
	 * 	Where the left hand side are variable names and right hand side are constants.
	 */
	string jsonString();

	/**
	 * The equality operator must also be overloaded when we want to assign a variable to top or bottom (or something else).
	 */
	void copy(Flow* rhs);

	/**
	 * The join function must be overloaded by the Flow subclasses.
	 */
	Flow* join(Flow* other);

	//You do not have to overload these constructors. You can create additional constructors with different signatures as well in your subclasses.
	RangeFlowSet();

	RangeFlowSet(string input);

	RangeFlowSet(RangeFlowSet *flow);
	//ConstantPropFlow(map<variable,int>());

	//Destructor must be virtual.
	virtual ~RangeFlowSet();

	//The Set Elements.
	//this is a map of variables to Ranges in the domain.
	map<string, RangeDomainElement > value;

};



#endif /* R_H_ */
