/*
 * PointerAnalysis.cpp
 *
 *  Created on: 2014-05-30
 *      Author: jtestard
 */



#include "PointerAnalysis.h"

StaticAnalysis::StaticAnalysis(Function &F){
	top = Flow("top");
	bottom = Flow("bottom");
	this->contextFlowGraph = new StaticAnalysis::ListNode(0);
	this->functionName = F.getName();
	buildCFG(F);
}

StaticAnalysis::~StaticAnalysis(){
	delete this->contextFlowGraph;
	//Might need to put something else here
}
