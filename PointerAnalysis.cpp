
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
		output = executeStoreInst(inFlow,inst);
		break;
	default:
		output = new PointerAnalysisFlow(inFlow);
		break;
	}
	return output;
}


PointerAnalysisFlow* PointerAnalysis::executeStoreInst(PointerAnalysisFlow* in, Instruction* instruction) {
	StoreInst* store = static_cast<StoreInst*>(instruction);
	//Check if right operand is a pointer
	if (store->getOperand(1)->getType()->isPointerTy()) {
		//Check if left & right operand names are non empty.
		if (store->getOperand(0)->getName()!="" && store->getOperand(1)->getName()!="") {
			PointerAnalysisFlow* f = new PointerAnalysisFlow();
			set<string> s;
			map<string, set<string> >value;
			s.insert(store->getOperand(0)->getName());
			value[store->getOperand(1)->getName()] = s;
			f->value = value;
			PointerAnalysisFlow* ff = static_cast<PointerAnalysisFlow*>(f->join(in));
			delete f;
			return ff;
		}
	}
	return in;
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
