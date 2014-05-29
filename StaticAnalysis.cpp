//
//  StaticAnalysisPass.cpp
//  
//
//  Created by Jules Testard on 22/05/2014.
//
//

#include "StaticAnalysis.h"


using namespace std;

StaticAnalysis::ListNode StaticAnalysis::getCFG(){
	return this->contextFlowGraph;
}

void StaticAnalysis::runWorklist(Module &M) {

}

void StaticAnalysis::buildCFG(Module &M){
	StaticAnalysis::ListNode node;
	this->contextFlowGraph.succs.push_back(node);
   	for (Module::iterator F = M.begin(), e = M.end() ; e != F ; ++F) {
		for(Function::iterator BB = F->begin(), E = F->end(); BB != E; ++BB) {

		}
   	}
}
//Prints out the graph data using BFS and avoiding cycles.
//Doesn't quite print out JSON yet, but some nice string representation.
string StaticAnalysis::JSONCFG() {
	stringstream ss;
	set<int> visited;
	string result = "";
	queue<ListNode> bfs;
	bfs.push(this->contextFlowGraph);
	while(!bfs.empty()) {
		ListNode current = bfs.front();
		ss << current.id << " : [";
		for(unsigned int i = 0 ; i < current.succs.size(); i++) {
			ss << current.succs[i].id;
			if (i+1<current.succs.size()) {
				ss << ",";
			}
			bfs.push(current.succs[i]);
		}
		ss << "]\n";
		bfs.pop();
	}
	return ss.str();
}

StaticAnalysis::StaticAnalysis(Module &M){
	this->contextFlowGraph = StaticAnalysis::ListNode();
	buildCFG(M);
}

StaticAnalysis::~StaticAnalysis(){

}
