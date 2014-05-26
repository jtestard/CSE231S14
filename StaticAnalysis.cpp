//
//  StaticAnalysisPass.cpp
//  
//
//  Created by Jules Testard on 22/05/2014.
//
//

#include "StaticAnalysis.h"
#include <iostream>

using namespace std;

StaticAnalysis::ListNode StaticAnalysis::getCFG(){
	return this->contextFlowGraph;
}

void StaticAnalysis::runWorklist(Module &M) {
	cout << "Worklist algorithm started...\n";
	buildCFG(M);
}

void StaticAnalysis::buildCFG(Module &M){
	cout << "Building context flow graph...\n";
//	StaticAnalysis::ListNode node();
//	this->contextFlowGraph = &node;
	this->contextFlowGraph.dirty = true;
	cout << "Dirty flag is : " << this->contextFlowGraph.dirty << "\n";
}

StaticAnalysis::StaticAnalysis(){
	this->contextFlowGraph = StaticAnalysis::ListNode();
}

StaticAnalysis::~StaticAnalysis(){

}
