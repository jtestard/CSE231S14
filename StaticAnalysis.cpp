//
//  StaticAnalysisPass.cpp
//  
//
//  Created by Jules Testard on 22/05/2014.
//
//

/**
 * Note : use the errs() instead of std::cout in this file to output to the console (if your name is not mike and you don't have a fancy debugger that
 * took hours to install :).
 */
#include "StaticAnalysis.h"

StaticAnalysis::ListNode* StaticAnalysis::getCFG(){
	return this->contextFlowGraph;
}

void StaticAnalysis::runWorklist() {

}

void StaticAnalysis::buildCFG(Function &F){
	Function::iterator BB = F.begin();
	BasicBlock::iterator BI = BB->begin();
	Instruction * firstInstruction = dyn_cast<Instruction>(BI);
	map<Instruction*,StaticAnalysis::ListNode*> helper;
	int counter = 1;

	//Build list nodes without successors.
   	for (Function::iterator e = F.end() ; e != BB ; ++BB) {
		BI = BB->begin();
		for(BasicBlock::iterator BE = BB->end(); BI != BE; ++BI){
			Instruction * instruction = dyn_cast<Instruction>(BI);
			StaticAnalysis::ListNode* node = new StaticAnalysis::ListNode(counter++);
			node->inst = instruction;
			helper.insert(pair<Instruction*,StaticAnalysis::ListNode*>(instruction,node));
		}
   	}

   	//Go through each key value mapping and update the successor list.
   	for (map<Instruction*,StaticAnalysis::ListNode*>::const_iterator it = helper.begin(); it != helper.end() ; ++it) {
   		Instruction* inst = it->first;
   		StaticAnalysis::ListNode* node = it->second;
   		if(isa<BranchInst>(inst)){
   			errs() << "Branch found!\n";
   			//Several successors
   			BranchInst * br = dyn_cast<BranchInst>(inst);
   			for (unsigned int i = 0 ; i < br->getNumSuccessors() ; i++) {
   				BasicBlock * bb = br->getSuccessor(i); //Get successor basic block
   				Instruction * nextInst = bb->getFirstNonPHIOrDbgOrLifetime(); // Gets the first legitimate instruction.
   				if (nextInst!=0)
   					if (helper.find(inst->getNextNode())!=helper.end())
   						node->succs.push_back(helper[nextInst]); //Add its node to the successors
   			}
   		} else {
   			//Only one successor
   			if (inst->getNextNode()!=0)
   				if (helper.find(inst->getNextNode())!=helper.end())
   					node->succs.push_back(helper[inst->getNextNode()]);
   			//Weird bug here. getNextNode can point to a basic block instead of an instruction. Should be taken care of by the key check.
   		}
   	}

   	//Make the root point to the first instruction
   	this->contextFlowGraph->succs.push_back(helper[firstInstruction]);
}

//Prints out the graph data using BFS and avoiding cycles.
//Doesn't quite print out JSON yet, but some nice string representation.
void StaticAnalysis::JSONCFG(raw_ostream &OS) {
	set<Instruction*> visited;
	queue<ListNode*> bfs;
	bfs.push(this->contextFlowGraph->succs[0]);
	OS << "[";
	while(!bfs.empty()) {
		ListNode* current = bfs.front();
		OS << "\"" << current->index << "\" : {\n"; //index
		OS << "\t\"representation\" : \"" << *(current->inst) << "\",\n"; //string representation
		OS << "\t\"out\" : [],\n"; //flow output
		OS << "\t\"successors\" : [";
		for(unsigned int i = 0 ; i < current->succs.size(); i++) {
			OS << current->succs[i]->index;
			if (i+1<current->succs.size()) {
				OS << ",";
			}
			if (visited.find(current->succs[i]->inst) == visited.end()) { //Add nodes to queue if never added before.
				bfs.push(current->succs[i]);
				visited.insert(current->succs[i]->inst);
			}
		}
		OS << "]\n},\n";
		bfs.pop();
	}
	OS << "]";
}

StringRef StaticAnalysis::getFunctionName(){
	return this->functionName;
}

StaticAnalysis::StaticAnalysis(Function &F){
	this->contextFlowGraph = new StaticAnalysis::ListNode(0);
	this->functionName = F.getName();
	buildCFG(F);
}

StaticAnalysis::~StaticAnalysis(){
	delete this->contextFlowGraph;
	//Might need to put something else here
}
