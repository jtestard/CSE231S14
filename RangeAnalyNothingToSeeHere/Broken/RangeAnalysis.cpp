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
#include "RangeFlowSet.h"
#include "RangeAnalysis.h"
/*
StaticAnalysis::ListNode* StaticAnalysis::getCFG(){
	return this->contextFlowGraph;
}

/**
 * The run worklist is not much more than a classic BFS.
 * Notice that it processes one instruction at a time. Processing multiple instructions at a time will be much harder.
 */
/*
void StaticAnalysis::runWorklist() {
	//We are using a queue for the worklist, but it could be any type of structure, really.
	queue<ListNode*> worklist;

	//Set each edge to bottom :
	//Top and bottom must be defined in order for the worklist to work.
	//This step uses the operator= from the Flow class.
	for (unsigned int i = 0; i < CFGEdges.size(); i++) {
		CFGEdges[i]->flow = bottom;
	}

	//Add each node to the worklist
	for (unsigned int i = 0 ; i < CFGNodes.size(); i++) {
		worklist.push(CFGNodes[i]);
	}

	while(!worklist.empty()){
		//It is assumed that any node popped from the worklist has a complete "in" flow.
		ListNode* current = worklist.front();

		//GET INPUT FLOW AND JOIN INTO UNIQUE FLOW
		vector<Flow> inputFlows;
		for (unsigned int i = 0 ; i < current->incoming.size() ; i++) {
			inputFlows.push_back(current->incoming[i]->flow);
		}

		//Since all edges have been initialized to a flow, inputFlows[0] never generates an exception.
		Flow in = inputFlows[0];
		for (unsigned int i = 1 ; i < inputFlows.size(); i++){
			in = in.join(inputFlows[i]);
		}

		//EXECUTE THE FLOW FUNCTION
		Flow out = executeFlowFunction(in,*(current->inst));

		//This will executed the flow function
				for(unsigned int i = 0 ; i < current->outgoing.size(); i++) {
					//GET NEW OUTPUT INFORMATION BY JOINING WITH EXISTING FLOW IN EDGE
					Flow new_out = out.join(current->outgoing[i]->flow);

					//IF INFORMATION HAS CHANGED, THEN PUSH TO WORKLIST
					if (!(new_out==current->outgoing[i]->flow)){
						current->outgoing[i]->flow = new_out;
						worklist.push(current->outgoing[i]->destination);
					}
				}
				worklist.pop();
	}
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

   	//Make the root point to the first instruction
   	this->contextFlowGraph = CFGNodes[0];
   	//Create incoming edge for the first node (does not exist otherwise.
   	StaticAnalysis::ListEdge* firstEdge = new StaticAnalysis::ListEdge(CFGNodes[0],CFGNodes[0]);
   	CFGNodes[0]->incoming.push_back(firstEdge);
   	CFGEdges.push_back(firstEdge);

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
	OS << "\"incoming\" : [\n";
	for (unsigned int i = 0 ; i < node->incoming.size() ; i++) {
		OS << "\t\t\t";
		StaticAnalysis::JSONEdge(OS,node->incoming[i]);
		if (i+1<node->incoming.size())
			OS << ",\n";
	}
	OS << "\n\t\t],\n";
	OS << "\t\t\"outgoing\" : [\n";
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
*/
/**
 * For basic static analysis, flow is just "assigned to top", which just means the basic string from the Flow general class will be top.
 * This method is expected to do much more when overloaded.
 */
//TO DO!!!!!!! NEEED CHANGED STRUCTURE
/*Flow RangeAnalysis::executeFlowFunction(Flow &in, Instruction &inst){
//	switch(instruction) {
//	case:
//
//	}
	return top;
}
*/

Flow* RangeAnalysis::initialize(){
	return new RangeFlowSet;
}

RangeAnalysis::RangeAnalysis(Function &F){
	//TO DO: Figure this out.
//	top = RangeFlowSet(Flow::TOP);//Should be changed by subclasses of Flow to an instance of the subclass
//	bottom = RangeFlowSet(Flow::BOTTOM);//Should be changed by subclasses of Flow to an instance of the subclass
	this->top = new RangeFlowSet;
	this->bottom = new RangeFlowSet;
	this->functionName = F.getName();
	buildCFG(F);
}


/*
 * This method is called by the run worklist algorithm.
 * It has the responsability to figure out what kind of instruction is being used and how to generate the output flow from the input flow for
 * that instruction.
 * It is expected this function will call other functions created by the subclasses to deal with each type of instruction.
 *
 * The output is a Flow that is the result of the processing of in with respect to instruction inst.
 */
Flow* RangeAnalysis::executeFlowFunction(Flow* in, Instruction* inst)
{
	RangeFlowSet* IN = (RangeFlowSet*)in;
	RangeFlowSet OUT;
	RangeFlowSet* pOUT = &OUT;
	//RangeFlowSet* OUT;
	//*OUT = new RangeFlowSet;

	string dest = inst->getName();	//

	pOUT->copy(in);	//Jules special.. requires this...

	//To get the instruction destination register, use getName()
	errs() << dest << "turn down for what?";
/*
	Value* a;

	unsigned Opcode = inst->getOpcode();
	switch(Opcode)
	{
		case Instruction::Add:
			a = inst->getOperand(0);
			// Checking if left operand is a constant
				if(ConstantInt *CILeft=dyn_cast<ConstantInt>(leftOperand)){
				        if(ConstantInt *CIRight=dyn_cast<ConstantInt>(rightOperand)){
				          int leftVal = CILeft->getZExtValue();
				          int rightVal = CIRight->getZExtValue();
				          int resVal = leftVal + rightVal;

				          errs() << leftVal << " "<< rightVal << "\n";
				          errs() << "outcome: " << resVal<< "\n";
	}*/

	return pOUT;
}

//END TO DO!!!!!

RangeAnalysis::~RangeAnalysis(){
	delete this->contextFlowGraph;
	//Might need to put something else here
	for (unsigned int i = 0 ; i < CFGNodes.size() ; i++) {
		delete CFGNodes[i];
	}
	for (unsigned int i = 0 ; i < CFGEdges.size() ; i++) {
		delete CFGEdges[i];
	}
}
