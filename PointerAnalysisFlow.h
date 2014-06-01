/*

 * Flow.h
 *
 *  Created on: 2014-05-29
 *      Author: jtestard
 */

#ifndef POINTER_ANALYSIS_FLOW_H_
#define POINTER_ANALYSIS_FLOW_H_
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>
#include "llvm/Support/raw_ostream.h"
#include "Flow.h"

using namespace std;
using namespace llvm;
/*
 * This class is the FLow class for the pointer analysis.
 * This is a May-Point-To Analysis.
 */

class PointerAnalysisFlow: public Flow {

public:

	//The equality operator is used by the worklist algorithm and must be overloaded by the analysis.
	bool equals(Flow* other);

	/* This method is used by the JSONCFG function of the analysis to output the graph in JSON format.
	 * It must output a proper representation of the flow in JSON format :
	 *
	 * 		{ "X" : "Z", "Z" : ["W","Y"] }
	 *
	 * 	Where the left hand side are variable names and right hand side are also variable names.
	 */
	string jsonString();

	/*
	 * The equality operator must also be overloaded when we want to assign a variable to top or bottom (or something else).
	 */
	void copy(Flow* rhs);

	/*
	 * The join function must be overloaded by the Flow subclasses.
	 */
	Flow* join(Flow* other);

	//This constructor initializes an empty map.
	PointerAnalysisFlow();

	//This constructor should be used for top or bottom.
	PointerAnalysisFlow(string input);

	//Required for type casting within overloaded functions.
	PointerAnalysisFlow(PointerAnalysisFlow* flow);

	~PointerAnalysisFlow();

	//Variables are represented as strings.
	map<string, set<string> > value;

};

#endif /* POINTER_ANALYSIS_FLOW_H_ */
