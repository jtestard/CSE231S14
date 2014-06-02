
//
//  StaticAnalysisPass.cpp
//  
//
//  Created by Jules Testard on 22/05/2014.
//
//

/*
 * Note : use the errs() instead of std::cout in this file to output to the console (if your name is not mike and you don't have a fancy debugger that
 * took hours to install :).
 */
#include "PointerAnalysis.h"
/*
 * For basic static analysis, flow is just "assigned to top", which just means the basic string from the Flow general class will be top.
 * This method is expected to do much more when overloaded.
 */
Flow* PointerAnalysis::executeFlowFunction(Flow *in, Instruction* inst){
	PointerAnalysisFlow* inFlow = static_cast<PointerAnalysisFlow*>(in);
	PointerAnalysisFlow * output;
	switch(inst->getOpcode()) {
	case 28 : //store instruction
		output = execute_X_equals_refY(inFlow,inst);
		break;
	case 27 : //Load instruction
		output = execute_X_equals_Y(inFlow,inst);
		break;
	default:
		output = new PointerAnalysisFlow(inFlow);
		break;
	}
	return output;
}

//StoreInst manages X = &Y
PointerAnalysisFlow* PointerAnalysis::execute_X_equals_refY(PointerAnalysisFlow* in, Instruction* instruction) {
	StoreInst* store = static_cast<StoreInst*>(instruction);
	PointerAnalysisFlow* f = new PointerAnalysisFlow(in);

	// X = &Y
	//Check if right operand is a pointer
	if (store->getOperand(1)->getType()->isPointerTy()) {
		//Check if left & right operand names are non empty.
		if (store->getOperand(0)->getName()!="" && store->getOperand(1)->getName()!="") {
			PointerAnalysisFlow* ff = new PointerAnalysisFlow();
			set<string> s;
			map<string, set<string> >value;
			s.insert(store->getOperand(0)->getName());
			value[store->getOperand(1)->getName()] = s;
			ff->value = value;
			PointerAnalysisFlow* tmp = static_cast<PointerAnalysisFlow*>(ff->join(f));
			delete ff;
			delete f;
			f = tmp;
		}
	}
	return f;
}

//Manages X = Y. Requires looking at two instructions to get all the necessary infomation.
//Managing this flow function requires spanning over two consecutive instructions.
PointerAnalysisFlow* PointerAnalysis::execute_X_equals_Y(PointerAnalysisFlow* in, Instruction* instruction) {
	LoadInst* load = static_cast<LoadInst*>(instruction);
	PointerAnalysisFlow* f = new PointerAnalysisFlow(in);
	Value* LO = load->getOperand(0);
	Value* RO = load->getNextNode()->getOperand(1);

	// X = Y
	// Check if both operands (LO & RO) are pointers.
	if (LO->getType()->isPointerTy() && RO->getType()->isPointerTy()) {
		if (LO->getName()!="" && RO->getName()!="") {
			//Everything LO points to, RO points to now as well.
			PointerAnalysisFlow* ff = new PointerAnalysisFlow();
			map<string, set<string> > value;
			//Get the set of everything LO points from the in and make RO point to it.
			value[RO->getName()] = in->value[LO->getName()];;
			ff->value = value;
			PointerAnalysisFlow* tmp = static_cast<PointerAnalysisFlow*>(ff->join(f));
			delete ff;
			delete f;
			f = tmp;
		}
	}
	return f;
}


Flow* PointerAnalysis::initialize(){
	return new PointerAnalysisFlow(PointerAnalysisFlow::BOTTOM);
}

PointerAnalysis::PointerAnalysis(Function &F) : StaticAnalysis() {
	this->top = new PointerAnalysisFlow(PointerAnalysisFlow::TOP);//Should be changed by subclasses of Flow to an instance of the subclass
	this->bottom = new PointerAnalysisFlow(PointerAnalysisFlow::BOTTOM);//Should be changed by subclasses of Flow to an instance of the subclass
	this->functionName = F.getName();
	buildCFG(F);
}
