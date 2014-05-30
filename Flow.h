/*
 * Flow.h
 *
 *  Created on: 2014-05-29
 *      Author: jtestard
 */

#ifndef FLOW_H_
#define FLOW_H_
#include <string>

using namespace std;
/**
 * This class is used to represent the information computed by static analyses.
 * It is a very general class but must overload the functions shown below.
 * It can be anything, but it should be most of the time have an attribute to map
 * each variable to the information computed by the analysis.
 */
class Flow {

public :

	//The equality operator is used by the worklist algorithm and must be overloaded by the analysis.
	virtual bool operator==(const Flow& other) const;

	/*
	 * This method is used by the JSONCFG function of the analysis to output the graph in JSON format.
	 * It must output a proper representation of the flow in JSON format. For example, for constant propagation :
	 *
	 * 		{ "X" : 20,	"Y" : 35, "Z" : 2.23, "W" : "d"	}
	 *
	 * 	Where the left hand side are variable names and right hand side are constants.
	 */
	virtual string jsonString();

	/**
	 * The equality operator must also be overloaded when we want to assign a variable to top or bottom (or something else).
	 */
	virtual Flow & operator=(const Flow &rhs);

	//You do not have to overload these constructors. You can create additional constructors with different signatures as well in your subclasses.
	Flow();
	Flow(string input);

	//Destructor must be virtual.
	virtual ~Flow();

	//This string is only here to demonstrate flows. It is not expected to be used by subclasses of Flow.
	string basic;

};



#endif /* FLOW_H_ */
