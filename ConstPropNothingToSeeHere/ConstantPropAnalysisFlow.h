/*
 * Flow.h
 *
 *  Created on: 2014-05-29
 *      Author: jtestard
 */

#ifndef CONSTANT_PROP_ANALYSIS_FLOW_H_
#define CONSTANT_PROP_ANALYSIS_FLOW_H_
#include <string>
#include <map>
#include "Flow.h"

using namespace std;
/**
 * This class is the FLow class for the constant propagation analysis.
 */
class ConstantpropAnalysisFlow : public Flow {

protected :

	//The equality operator is used by the worklist algorithm and must be overloaded by the analysis.
	bool equals(Flow& other);

	/*
	 * This method is used by the JSONCFG function of the analysis to output the graph in JSON format.
	 * It must output a proper representation of the flow in JSON format :
	 *
	 * 		{ "X" : 1, "Z" : 5.5 }
	 *
	 * 	Where the left hand side are variable names and right hand side are floats.
	 */
	string jsonString();

	/**
	 * The equality operator must also be overloaded when we want to assign a variable to top or bottom (or something else).
	 */
	void copy(Flow &rhs);

	/**
	 * The join function must be overloaded by the Flow subclasses.
	 */
	Flow join(Flow &other);

	//This constructor initializes an empty map.
	ConstantpropAnalysisFlow();

	//This constructor should be used for top or bottom.
	ConstantpropAnalysisFlow(string input);

	ConstantpropAnalysisFlow(Flow &flow);

	~ConstantpropAnalysisFlow();

	//This string should only be used to represent TOP or BOTTOM.
	string basic;

	//Variables are represented as strings.
	map<string,float > value;

};



#endif /* POINTER_ANALYSIS_FLOW_H_ */
