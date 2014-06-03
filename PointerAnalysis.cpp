
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
		//If one load followed by one store, then is X = Y
		//If two loads followed by one store, then is *X = Y
		{
			Instruction* nInst = inst->getNextNode();
			Instruction* nnInst = nInst->getNextNode();
			if(isa<StoreInst>(nInst)) {
				output = execute_X_equals_Y(inFlow,inst);
			} else if (isa<LoadInst>(nInst) && isa<StoreInst>(nnInst)) {
				output = execute_ptrX_equals_Y(inFlow,inst);
			}
			break;
		}
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
			// X now points to Y.
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
	Value* Y = load->getOperand(0); //RO
	Value* X = load->getNextNode()->getOperand(1); //LO

	// X = Y
	// Check if both operands (X & Y) are pointers.
	if (Y->getType()->isPointerTy() && X->getType()->isPointerTy()) {
		if (Y->getName()!="" && X->getName()!="") {
			//Everything Y points to, X points to now as well.
			PointerAnalysisFlow* ff = new PointerAnalysisFlow();
			map<string, set<string> > value;
			//Get the set of everything Y points from the in and make X point to it.
			value[X->getName()] = in->value[Y->getName()];;
			ff->value = value;
			PointerAnalysisFlow* tmp = static_cast<PointerAnalysisFlow*>(ff->join(f));
			delete ff;
			delete f;
			f = tmp;
		}
	}
	return f;
}

//Manages *X = Y
PointerAnalysisFlow* PointerAnalysis::execute_ptrX_equals_Y(PointerAnalysisFlow* in, Instruction* instruction){
	PointerAnalysisFlow* f = new PointerAnalysisFlow(in);
	Value* Y = instruction->getOperand(0); //RO
	Value* X = instruction->getNextNode()->getOperand(0); //LO
	
	//Check that both operands are pointers.
	if (Y->getType()->isPointerTy() && X->getType()->isPointerTy()) {
		if (Y->getName()!="" && X->getName()!="") {
			//Everything Y points to, *X points to now as well.
			PointerAnalysisFlow* ff = new PointerAnalysisFlow();
			set<string> pointedByX = in->value[X->getName()];
			map<string, set<string> > value;
			for (set<string>::iterator it = pointedByX.begin() ; it != pointedByX.end() ; it++) {
				//if X points to W, then W points to everything Y points to.
				string W = *it;
				value[W] = in->value[Y->getName()];
			}
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
