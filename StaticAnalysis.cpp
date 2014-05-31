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
//	//We are using a queue for the worklist, but it could be any type of structure, really.
//	queue<ListNode*> worklist;
//
//	//Set each edge to bottom :
//	//Top and bottom must be defined in order for the worklist to work.
//	//This step uses the operator= from the Flow class.
//	for (unsigned int i = 0; i < CFGarray.size(); i++) {
//		CFGarray[i]->in = bottom;
//		worklist.push(CFGarray[i]);
//	}
//
//	while(!worklist.empty()){
//		//It is assumed that any node popped from the worklist has a complete "in" flow.
//		ListNode* current = worklist.front();
//
//		//This will executed the flow function
//		for(unsigned int i = 0 ; i < current->succs.size(); i++) {
//			//Execute flow function and push back on the queue if the flows are different.
//			//This step uses the operator== from the Flow class.
//			if(!(current->in==current->succs[i]->in)){
//				//The successor's input is updated with the processing of the current node's input
//				//by the flow function corresponding to the instruction.
//				//In case of loops, the successor's input will not be empty at this step. It must
//				//be joined with the result of the processing of the current node's input.
//				executeFlowFunction(current->in, *(current->inst), current->succs[i]->in);
//				worklist.push(current->succs[i]);
//			}
//		}
//
//		worklist.pop();
//	}
}

void StaticAnalysis::buildCFG(Function &F){
	Function::iterator BB = F.begin();
	BasicBlock::iterator BI = BB->begin();
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
			CFGNodes.push_back(node);
		}
   	}

   	//Go through each key value mapping and update the successor list.
   	for (map<Instruction*,StaticAnalysis::ListNode*>::const_iterator it = helper.begin(); it != helper.end() ; ++it) {
   		Instruction* inst = it->first;
   		StaticAnalysis::ListNode* node = it->second;
   		if(isa<BranchInst>(inst)){
   			//Several outgoing
   			BranchInst * br = dyn_cast<BranchInst>(inst);
   			for (unsigned int i = 0 ; i < br->getNumSuccessors() ; i++) {
   				BasicBlock * bb = br->getSuccessor(i); //Get successor basic block
   				Instruction * nextInst = bb->getFirstNonPHIOrDbgOrLifetime(); // Gets the first legitimate instruction.
   				if (nextInst!=0)
   					if (helper.find(nextInst)!=helper.end()) {
   	   					StaticAnalysis::ListNode* nextNode = helper[nextInst];
   	   					StaticAnalysis::ListEdge* edge = new StaticAnalysis::ListEdge(node,nextNode);
   	   					node->outgoing.push_back(edge);
   	   					nextNode->incoming.push_back(edge);
   	   					CFGEdges.push_back(edge);
   					}

   			}
   		} else {
   			//Only one outgoing
   			if (inst->getNextNode()!=0)
   				if (helper.find(inst->getNextNode())!=helper.end()) {
   					StaticAnalysis::ListNode* nextNode = helper[inst->getNextNode()];
   					StaticAnalysis::ListEdge* edge = new StaticAnalysis::ListEdge(node,nextNode);
   					node->outgoing.push_back(edge);
   					nextNode->incoming.push_back(edge);
   					CFGEdges.push_back(edge);
   				}
   			//Weird bug here. getNextNode can point to a basic block instead of an instruction. Should be taken care of by the key check.
   		}
   	}

   	//Make the root point to the first instruction
   	this->contextFlowGraph = CFGNodes[0];
}

//Prints out the graph data using BFS and avoiding cycles.
//Doesn't quite print out JSON yet, but some nice string representation.
void StaticAnalysis::JSONCFG(raw_ostream &OS) {
	//The graph data representation is now edge-based.
	OS << "[\n";
	for (unsigned int i = 0; i < CFGNodes.size() ; i++) {
		StaticAnalysis::JSONNode(OS,CFGNodes[i]);
		if(i+1 < CFGNodes.size())
			OS << ",\n";
	}
	OS << "\n]";
}

void StaticAnalysis::JSONEdge(raw_ostream &OS, ListEdge* edge) {
	OS << "{\"Edge\" : " << "[" << edge->source->index << "," << edge->destination->index << "],";
	OS << "\"Flow\" : " << edge->flow.jsonString() << "}";
}

void StaticAnalysis::JSONNode(raw_ostream &OS, ListNode* node) {
	OS << "\t\"" << node->index << "\" : {\n\t\t";
	OS << "\"representation\" : \"" << *(node->inst) << "\",\n\t\t";
	OS << "\"incoming\" : [\n\t\t";
	for (unsigned int i = 0 ; i < node->incoming.size() ; i++) {
		OS << "\t\t\t";
		StaticAnalysis::JSONEdge(OS,node->incoming[i]);
		if (i+1<node->incoming.size())
			OS << ",\n";
	}
	OS << "\n\t\t],\n";
	OS << "\t\t\"outgoing\" : [\n\t\t";
	for (unsigned int i = 0 ; i < node->outgoing.size() ; i++) {
		OS << "\t\t\t";
		StaticAnalysis::JSONEdge(OS,node->outgoing[i]);
		if (i+1<node->outgoing.size())
			OS << ",\n";
	}
	OS << "\n\t\t]\n";
	OS << "\t}";
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
	top = Flow(Flow::TOP);//Should be changed by subclasses of Flow to an instance of the subclass
	bottom = Flow(Flow::BOTTOM);//Should be changed by subclasses of Flow to an instance of the subclass
	this->functionName = F.getName();
	buildCFG(F);
}

StaticAnalysis::~StaticAnalysis(){
	delete this->contextFlowGraph;
	//Might need to put something else here
	for (unsigned int i = 0 ; i < CFGNodes.size() ; i++) {
		delete CFGNodes[i];
	}
	for (unsigned int i = 0 ; i < CFGEdges.size() ; i++) {
		delete CFGEdges[i];
	}
}
