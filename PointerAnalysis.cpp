
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
 *
 *
 *
 *
 *
 */
#include "PointerAnalysis.h"
/*
 * For basic static analysis, flow is just "assigned to top", which just means the basic string from the Flow general class will be top.
 * This method is expected to do much more when overloaded.
 * We use the presence of names in the instructions in LLVM.
 */
Flow* PointerAnalysis::executeFlowFunction(Flow *in, Instruction *inst, int NodeId){
	PointerAnalysisFlow* inFlow = static_cast<PointerAnalysisFlow*>(in);
	PointerAnalysisFlow * output;
	switch(inst->getOpcode()) {
	case 29 : // getelementptr instruction. Used when incrementing a pointer.
		output = execute_X_equals_NULL(inFlow,inst);
		break;
	case 28 : // Store instructions are X = &Y
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
				if (inst->getOperand(0)->getName()!="" && nInst->getOperand(0)->getName()!="")
					output = execute_ptrX_equals_Y(inFlow,inst);
				else if (inst->getOperand(0)->getName()!="" && nnInst->getOperand(1)->getName()!="")
					output = execute_X_equals_ptrY(inFlow,inst);
				else
					output = new PointerAnalysisFlow(inFlow);
			} else {
				output = new PointerAnalysisFlow(inFlow);
			}
			break;
		}
	default:
		output = new PointerAnalysisFlow(inFlow);
		break;
	}
	return output;
}

/**
 * C CODE : X = &Y
 * LLVM CODE :
 * store float* %Y, float** %X, align 4
 */
PointerAnalysisFlow* PointerAnalysis::execute_X_equals_refY(PointerAnalysisFlow* in, Instruction* instruction) {
	//Check that left operand is not null.
	if (isa<ConstantPointerNull>(instruction->getOperand(0))) {
		return execute_X_equals_NULL(in,instruction);
	}

	StoreInst* store = static_cast<StoreInst*>(instruction);
	PointerAnalysisFlow* f = new PointerAnalysisFlow(in);
	// X = &Y
	//Check if right operand is a pointer
	if (store->getOperand(1)->getType()->isPointerTy()) {
		//Check if left & right operand names are non empty.
		if (store->getOperand(0)->getName()!="" && store->getOperand(1)->getName()!="") {
			if (madeByLLVM(store->getOperand(0)->getName())) {
				return f;
			}
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

/**
 * C CODE : X = Y
 * LLVM CODE :
 * %0 = load float** %Y, align 4
 * store float* %0, float** %X, align 4
 */
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

/**
 * C CODE : *X = Y
 * LLVM CODE :
 * %0 = load float** %Y, align 4
 * %1 = load float*** %X, align 4
 * store float* %0, float** %1, align 4
 */
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

/**
 * C CODE : X = *Y
 * LLVM CODE :
 * %0 = load float*** %Y, align 4
 * %1 = load float** %0, align 4
 * store float* %1, float** %X, align 4
 */
PointerAnalysisFlow* PointerAnalysis::execute_X_equals_ptrY(PointerAnalysisFlow* in, Instruction* instruction) {
	PointerAnalysisFlow* f = new PointerAnalysisFlow(in);
	Value* Y = instruction->getOperand(0);
	Value* X = instruction->getNextNode()->getNextNode()->getOperand(1);
	if (Y->getType()->isPointerTy() && X->getType()->isPointerTy()) {
		if (Y->getName()!="" && X->getName()!="") {
			PointerAnalysisFlow* ff = new PointerAnalysisFlow();
			//For all W pointed by Y, X points to Z, where Z is pointed by W.
			set<string> pointedByY = in->value[Y->getName()]; //Set of Ws.
			for (set<string>::iterator W = pointedByY.begin(); W != pointedByY.end() ; W++) {
				for (set<string>::iterator Z = in->value[*W].begin() ; Z != in->value[*W].end() ; Z++) {
					ff->value[X->getName()].insert(*Z);
				}
			}
			PointerAnalysisFlow* tmp = static_cast<PointerAnalysisFlow*>(ff->join(f));
			delete ff;
			delete f;
			f = tmp;
		}
	}
	return f;
}

/**
 * C CODE : X = NULL
 * store float* null, float** %X, align 4
 * C CODE : X = Y+i
 * %add.ptr = getelementptr inbounds float** %Y, i32 i
 * store float** %add.ptr, float** %X, align 4
 */
PointerAnalysisFlow* PointerAnalysis::execute_X_equals_NULL(PointerAnalysisFlow* in, Instruction* instruction) {
	PointerAnalysisFlow* f = new PointerAnalysisFlow(in);
	// if C CODE : X = Y+i
	if (isa<GetElementPtrInst>(instruction)) {
		Value* X = instruction->getNextNode()->getOperand(1);
		if (X->getType()->isPointerTy() && X->getName() != "") {
			f->value.erase(X->getName());
		}
		return f;
	}
	//if C CODE : X = NULL
	else if (isa<StoreInst>(instruction)) {
		Value* X = instruction->getOperand(1);
		if (X->getType()->isPointerTy() && X->getName() != "") {
			f->value.erase(X->getName());
		}
		return f;
	} else {
		return f;
	}
}

bool PointerAnalysis::madeByLLVM(string name) {
	if(name.substr(name.find_last_of(".") + 1) == "ptr") {
		return true;
	} else {
		return false;
	}
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
