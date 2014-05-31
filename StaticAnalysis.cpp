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

/**
 * The run worklist is not much more than a classic BFS.
 * Notice that it processes one instruction at a time. Processing multiple instructions at a time will be much harder.
 */
void StaticAnalysis::runWorklist() {
	queue<ListNode*> worklist;
	ListNode* firstNode = this->contextFlowGraph->succs[0];

	//Top must be defined in order for the worklist to work.
	//This step uses the operator= from the Flow class.
	firstNode->in = top;

	worklist.push(firstNode);
	while(!worklist.empty()){
		//It is assumed that any node popped from the worklist has a complete "in" flow.
		ListNode* current = worklist.front();

		//This will executed the flow function
		for(unsigned int i = 0 ; i < current->succs.size(); i++) {
			//Execute flow function and push back on the queue if the flows are different.
			//This step uses the operator== from the Flow class.
			if(!(current->in==current->succs[i]->in)){
				//The successor's input is updated with the processing of the current node's input
				//by the flow function corresponding to the instruction.
				//In case of loops, the successor's input will not be empty at this step. It must
				//be joined with the result of the processing of the current node's input.
				executeFlowFunction(current->in, *(current->inst), current->succs[i]->in);
				worklist.push(current->succs[i]);
			}
		}

		worklist.pop();
	}
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
   			//Several successors
   			BranchInst * br = dyn_cast<BranchInst>(inst);
   			for (unsigned int i = 0 ; i < br->getNumSuccessors() ; i++) {
   				BasicBlock * bb = br->getSuccessor(i); //Get successor basic block
   				Instruction * nextInst = bb->getFirstNonPHIOrDbgOrLifetime(); // Gets the first legitimate instruction.
   				if (nextInst!=0)
   					if (helper.find(nextInst)!=helper.end())
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
		OS << "\t\"in\" : " << current->in.jsonString() << ",\n"; //flow output
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

/**
 * For basic static analysis, flow is just "assigned to top", which just means the basic string from the Flow general class will be top.
 * This method is expected to do much more when overloaded.
 */
void StaticAnalysis::executeFlowFunction(Flow &in, Instruction &inst, Flow &out){
	out = top;
}

StaticAnalysis::StaticAnalysis(Function &F){
	top = Flow("top");//Ahould be changed by subclasses of Flow
	bottom = Flow("bottom");//Ahould be changed by subclasses of Flow
	this->contextFlowGraph = new StaticAnalysis::ListNode(0);
	this->functionName = F.getName();
	buildCFG(F);
}

StaticAnalysis::~StaticAnalysis(){
	delete this->contextFlowGraph;
	//Might need to put something else here
}
