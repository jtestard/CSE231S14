//
//  StaticAnalysisPass.cpp
//  
//
//  Created by Jules Testard on 22/05/2014.
//
//

#include "StaticAnalysis.hpp"

using namespace llvm;
using namespace std;

StaticAnalysis::ListNode StaticAnalysis::getCFG(){
	return this->contextFlowGraph;
}

void StaticAnalysis::runWorklist(Module &M) {
	buildCFG(M);
}

void StaticAnalysis::buildCFG(Module &M){
	StaticAnalysis::ListNode root();
}
