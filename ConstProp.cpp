#include "ConstProp.h"

ConstProp::ConstProp(Function &F){

	top = Flow("top");

	bottom = Flow("bottom");
	this->contextFlowGraph = new StaticAnalysis::ListNode(0);
	this->functionName = F.getName();
	buildCFG(F);
}



// Why?
/*
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
*/
