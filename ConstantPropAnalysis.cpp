//
//  ConstantPropAnalysisPass.cpp
//  
//
//  Created by Costas Zarifis on 22/05/2014.
//
//

/*
 * Note : use the errs() instead of std::cout in this file to output to the console (if your name is not mike and you don't have a fancy debugger that
 * took hours to install :).
 */
#include "ConstantPropAnalysis.h"

// You can actually check out the opcodes in this address:
// 	http://code.woboq.org/userspace/llvm/include/llvm/IR/Instruction.def.html

#define ADD 8 //This is the opcode for the add instruction
#define FADD 9 //This is the opcode for the add instruction
#define SUB 10 //This is the opcode for the sub instruction
#define FSUB 11 //This is the opcode for the floating point sub instruction
#define MUL 12 //This is the opcode for the mul instruction
#define FMUL 13 //This is the opcode for the floating point mul instruction
#define SDIV 15 //This is the opcode for the signed div instruction
#define FDIV 16 //This is the opcode for the float div instruction
#define UREM 17 //This is the opcode for the unsigned mod instruction
#define SREM 18 //This is the opcode for the signed mod instruction
#define FREM 19 //This is the opcode for the floating point mod instruction

#define SHL 20 //This is the opcode for the Shift left (logical) instruction
#define LSHR 21 //This is the opcode for the Shift right (logical) instruction
#define ASHR 22 //This is the opcode for the Shift right (arithmetic) instruction

// MEMORY/POINTER OPCODES RETURN TOP
#define ALLOCA 26
#define LOAD 27
#define STORE 28
#define ELPTR 29
#define FENCE 30
#define ATCMP 31
#define ATRMW 32
#define PTRTOINT 42
#define INTTOPTR 43


#define TRUNC 33 // Truncate integers
#define ZEXT 34 // Zero extend integers
#define SEXT 35 // Sign extend integers
#define FPTOUI 36 //This is the opcode for the int to float cast instruction
#define FPTOSI 37 //This is the opcode for the float to integer cast instruction
#define UITOFP 38 //UInt -> floating point
#define SITOFP 39 // SInt -> floating point
#define FPTRUNC 40 //Truncate floating point
#define FPEXT 41 // Extend floating point

#define PHI 48 // Extend floating point

/*
 * For basic static analysis, flow is just "assigned to top", which just means the basic string from the Flow general class will be top.
 * This method is expected to do much more when overloaded.
 */
Flow* ConstantPropAnalysis::executeFlowFunction(Flow *in, Instruction *inst, int NodeId){
	ConstantPropAnalysisFlow* inFlow =
			static_cast<ConstantPropAnalysisFlow*>(in);
	ConstantPropAnalysisFlow * output;

	switch (inst->getOpcode()) {

	case ADD:
	case SUB:
	case MUL:
	case SDIV:
	case SREM:
	case SHL:
	case LSHR:
	case ASHR:
		output = executeOpInst(inFlow, inst, inst->getOpcode());
		break;
	case FADD:
	case FSUB:
	case FMUL:
	case FDIV:
	case FREM:
		output = executeFOpInst(inFlow, inst, inst->getOpcode());
		break;

	case TRUNC:
	case ZEXT:
	case SEXT:
	case FPTOSI:
	case FPTOUI:
	case UITOFP:
	case SITOFP:
	case FPTRUNC:
	case FPEXT:
		output = executeCastInst(inFlow, inst);
		break;
	case PHI:
		output = executePhiInst(inFlow, inst);
		break;


	case ALLOCA:
	case LOAD:
	case STORE:
	case ELPTR:
	case FENCE:
	case ATCMP:
	case ATRMW:
	case PTRTOINT:
	case INTTOPTR:
		// If I reach this point it means that I have a pointer...
		// Return TOP!
		output = retTop();
		break;


	default:
		output = new ConstantPropAnalysisFlow(inFlow);
		break;
	}
	//errs() << "Instruction : " << *inst << ", Flow value : " << output->jsonString() << "\n";
	return output;
}

ConstantPropAnalysisFlow* ConstantPropAnalysis::retTop() {
	ConstantPropAnalysisFlow* f = new ConstantPropAnalysisFlow("top");
	f->basic="top";

	return f;
}

ConstantPropAnalysisFlow* ConstantPropAnalysis::executeCastInst(
		ConstantPropAnalysisFlow* in, Instruction* instruction) {

	ConstantPropAnalysisFlow* f = new ConstantPropAnalysisFlow(in);
	//Value *leftOperand = instruction->getOperand(0);
	//Value *rightOperand = instruction->getOperand(1);
	map<string, float> value;
	Value *retVal = instruction;
	string regName = retVal->getName();
	 CastInst *castInst = dyn_cast<CastInst>(instruction);


	Value* casting = instruction->getOperand(0); //RO


	if (!dyn_cast<Constant>(retVal)) {

		if (!dyn_cast<Constant>(casting)) {
			// Cool they are both variables. We just need to forward the value
			if (f->value.find(casting->getName()) == f->value.end()) {
				// Oh no! Read the error message!

			} else {
				// Hmm, I guess we're good...

				float forwardVal = f->value.find(casting->getName())->second;
				ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();

				Type* ttype = castInst->getDestTy();

				if (ttype->isDoubleTy())
					forwardVal = (float) forwardVal;
				else if(ttype->isFloatingPointTy())
					forwardVal = (float) forwardVal;
				else if (ttype->isIntegerTy()){
					forwardVal = (int) forwardVal;
				}

				value[retVal->getName()] = forwardVal;

				ff->value = value;
				ConstantPropAnalysisFlow* tmp =
						static_cast<ConstantPropAnalysisFlow*>(ff->join(f));
				delete ff;
				delete f;
				f = tmp;
			}

		} else {

			// Hmm, I guess we're good...
			if (ConstantFP *cfp = dyn_cast<ConstantFP>(casting)) {

				float forwardVal = cfp->getValueAPF().convertToFloat();

				//float forwardVal = f->value.find(casting->getName())->second;
				ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
				value[retVal->getName()] = forwardVal;
				ff->value = value;
				ConstantPropAnalysisFlow* tmp =
						static_cast<ConstantPropAnalysisFlow*>(ff->join(f));
				delete ff;
				delete f;
				f = tmp;
			} else if (ConstantInt *cfp = dyn_cast<ConstantInt>(casting)) {
				float forwardVal = cfp->getZExtValue();
				ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
				value[retVal->getName()] = forwardVal;
				ff->value = value;
				ConstantPropAnalysisFlow* tmp =
						static_cast<ConstantPropAnalysisFlow*>(ff->join(f));
				delete ff;
				delete f;
				f = tmp;

			}

		}

	}
	return f;

}

float ConstantPropAnalysis::computeOp(float leftVal, float rightVal,
		unsigned opcode) {

	float resVal = 0;
	int ASHRVAL, ASHRMASK;
	switch (opcode) {

	case ADD:
	case FADD:
		resVal = leftVal + rightVal;
		break;
	case SUB:
	case FSUB:
		resVal = leftVal - rightVal;
		break;
	case FDIV:
	case SDIV:
		resVal = leftVal / rightVal;
		break;
	case FMUL:
	case MUL:
		resVal = leftVal * rightVal;
		break;
	case FREM:
	case SREM:
		resVal = (int) leftVal % (int) rightVal;
		break;
	case SHL:
		resVal = (int) leftVal << (int) rightVal;
		break;
	case LSHR:
		resVal = (int) leftVal >> (int) rightVal;
		break;
	case ASHR:
		ASHRMASK = (int)leftVal;
		ASHRMASK &= 0x80000000;
		ASHRVAL = (int)leftVal;
		ASHRVAL &= 0x7fffffff;
		ASHRMASK |=( ASHRVAL >> (int) rightVal);
		resVal = (int)ASHRMASK;
		break;
	}

	return resVal;

}

ConstantPropAnalysisFlow* ConstantPropAnalysis::executePhiInst(
		ConstantPropAnalysisFlow* in, Instruction* instruction) {

	ConstantPropAnalysisFlow* f = new ConstantPropAnalysisFlow(in);
	Value *leftOperand = instruction->getOperand(0);
	Value *rightOperand = instruction->getOperand(1);
	map<string, float> value;
	Value *K = instruction;
	string regName = K->getName();

	// Ok, cool! Both the right and the left operand is a variable...
	if ((f->value.find(leftOperand->getName()) == f->value.end())
			| (f->value.find(rightOperand->getName()) == f->value.end())) {
		// Oh no! Read the error message!

	} else {
		// Hmm, I guess we're good...
		float leftVal = f->value.find(leftOperand->getName())->second;

		float rightVal = f->value.find(rightOperand->getName())->second;
		errs() << "leftVal: " << leftVal << "rightVal" << rightVal << "\n";

		// If the variables are not the same in the two branches then
		// we can't propagate the constant.
		if (leftVal == rightVal){

			float resVal = leftVal;
			ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
			value[K->getName()] = resVal;
			ff->value = value;
			ConstantPropAnalysisFlow* tmp =
					static_cast<ConstantPropAnalysisFlow*>(ff->join(f));
			delete ff;
			delete f;
			f = tmp;
		}

	}

	return f;
}

ConstantPropAnalysisFlow* ConstantPropAnalysis::executeFOpInst(
		ConstantPropAnalysisFlow* in, Instruction* instruction,
		unsigned opcode) {

	ConstantPropAnalysisFlow* f = new ConstantPropAnalysisFlow(in);
	Value *leftOperand = instruction->getOperand(0);
	Value *rightOperand = instruction->getOperand(1);
	map<string, float> value;
	Value *K = instruction;
	string regName = K->getName();

// Checking if left operand is a constant
	if (ConstantFP *CILeft = dyn_cast<ConstantFP>(leftOperand)) {

		if (ConstantFP *CIRight = dyn_cast<ConstantFP>(rightOperand)) {
			// Cool they are both constants.

			float leftVal = CILeft->getValueAPF().convertToFloat();
			float rightVal = CIRight->getValueAPF().convertToFloat();

			float resVal = computeOp(leftVal, rightVal, opcode);

			ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
			//errs() << leftVal << " " << rightVal << "\n";
			//errs() << "outcome: " << resVal << "\n";
			value[K->getName()] = resVal;
			ff->value = value;
			ConstantPropAnalysisFlow* tmp =
					static_cast<ConstantPropAnalysisFlow*>(ff->join(f));
			delete ff;
			delete f;
			f = tmp;
		} else {
			// ok so the right operand is a variable
			if (f->value.find(rightOperand->getName()) == f->value.end()) {
				// Oh no! Read the error message!
			}

			else {

				// Hmm, I guess we're good...
				float leftVal = CILeft->getValueAPF().convertToFloat();
				float rightVal = f->value.find(rightOperand->getName())->second;

				float resVal = computeOp(leftVal, rightVal, opcode);

				ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
				//errs() << leftVal << " " << rightVal << "\n";
				//errs() << "outcome: " << resVal << "\n";
				value[K->getName()] = resVal;
				ff->value = value;
				ConstantPropAnalysisFlow* tmp =
						static_cast<ConstantPropAnalysisFlow*>(ff->join(f));
				delete ff;
				delete f;
				f = tmp;
			}
		}
	} else {
		// So, the left part of the addition is a variable. We'll have to check the input set to get the value
		// this variable has at the moment.
		if (ConstantFP *CIRight = dyn_cast<ConstantFP>(rightOperand)) {
			// Ok, cool! the right part is a constant...
			//leftOperand->getName()

			//int leftVal = CILeft->getZExtValue();

			if (f->value.find(leftOperand->getName()) == f->value.end()) {

			} else {
				// Hmm, I guess we're good...

				float leftVal = f->value.find(leftOperand->getName())->second;
				float rightVal = CIRight->getValueAPF().convertToFloat();
				//float resVal = leftVal + rightVal;

				float resVal = computeOp(leftVal, rightVal, opcode);
				ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
				//errs() << leftVal << " " << rightVal << "\n";
				//errs() << "outcome: " << resVal << "\n";
				value[K->getName()] = resVal;
				ff->value = value;
				ConstantPropAnalysisFlow* tmp =
						static_cast<ConstantPropAnalysisFlow*>(ff->join(f));
				delete ff;
				delete f;
				f = tmp;
			}
		} else {

			// Ok, cool! Both the right and the left operand is a variable...
			if ((f->value.find(leftOperand->getName()) == f->value.end())
					| (f->value.find(rightOperand->getName()) == f->value.end())) {

			} else {
				// Hmm, I guess we're good...
				float leftVal = f->value.find(leftOperand->getName())->second;

				float rightVal = f->value.find(rightOperand->getName())->second;
				float resVal = computeOp(leftVal, rightVal, opcode);
				ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
				//errs() << leftVal << " " << rightVal << "\n";
				//errs() << "outcome: " << resVal << "\n";
				value[K->getName()] = resVal;
				ff->value = value;
				ConstantPropAnalysisFlow* tmp =
						static_cast<ConstantPropAnalysisFlow*>(ff->join(f));
				delete ff;
				delete f;
				f = tmp;

			}

		}

	}
	return f;
}

ConstantPropAnalysisFlow* ConstantPropAnalysis::executeOpInst(
		ConstantPropAnalysisFlow* in, Instruction* instruction,
		unsigned opcode) {

	ConstantPropAnalysisFlow* f = new ConstantPropAnalysisFlow(in);
	Value *leftOperand = instruction->getOperand(0);
	Value *rightOperand = instruction->getOperand(1);
	map<string, float> value;
	Value *K = instruction;
	string regName = K->getName();
//errs() << "Instruction : " << regName << " left " << leftOperand->getName()
//		<< " right " << rightOperand->getName() << "\n";

// Checking if left operand is a constant
	if (ConstantInt *CILeft = dyn_cast<ConstantInt>(leftOperand)) {

		if (ConstantInt *CIRight = dyn_cast<ConstantInt>(rightOperand)) {
			// Cool they are both constants.

			float leftVal = CILeft->getZExtValue();
			float rightVal = CIRight->getZExtValue();

			float resVal = computeOp(leftVal, rightVal, opcode);

			//float resVal = leftVal + rightVal;
			ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
			//errs() << leftVal << " " << rightVal << "\n";
			//errs() << "outcome: " << resVal << "\n";
			value[K->getName()] = resVal;
			ff->value = value;
			ConstantPropAnalysisFlow* tmp =
					static_cast<ConstantPropAnalysisFlow*>(ff->join(f));
			delete ff;
			delete f;
			f = tmp;
		} else {
			// ok so the right operand is a variable
			if (f->value.find(rightOperand->getName()) == f->value.end()) {

			}

			else {

				// Hmm, I guess we're good...
				float leftVal = CILeft->getZExtValue();
				float rightVal = f->value.find(rightOperand->getName())->second;
				float resVal = computeOp(leftVal, rightVal, opcode);
				//float resVal = leftVal + rightVal;

				ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
				//errs() << leftVal << " " << rightVal << "\n";
				//errs() << "outcome: " << resVal << "\n";
				value[K->getName()] = resVal;
				ff->value = value;
				ConstantPropAnalysisFlow* tmp =
						static_cast<ConstantPropAnalysisFlow*>(ff->join(f));
				delete ff;
				delete f;
				f = tmp;
			}
		}
	} else {
		// So, the left part of the addition is a variable. We'll have to check the input set to get the value
		// this variable has at the moment.
		if (ConstantInt *CIRight = dyn_cast<ConstantInt>(rightOperand)) {
			// Ok, cool! the right part is a constant...
			//leftOperand->getName()

			//int leftVal = CILeft->getZExtValue();

			if (f->value.find(leftOperand->getName()) == f->value.end()) {

			} else {
				// Hmm, I guess we're good...

				float leftVal = f->value.find(leftOperand->getName())->second;
				float rightVal = CIRight->getZExtValue();
				float resVal = computeOp(leftVal, rightVal, opcode);

				//float resVal = leftVal + rightVal;
				ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
				//errs() << leftVal << " " << rightVal << "\n";
				//errs() << "outcome: " << resVal << "\n";
				value[K->getName()] = resVal;
				ff->value = value;
				ConstantPropAnalysisFlow* tmp =
						static_cast<ConstantPropAnalysisFlow*>(ff->join(f));
				delete ff;
				delete f;
				f = tmp;
			}
		} else {

			// Ok, cool! Both the right and the left operand is a variable...
			if ((f->value.find(leftOperand->getName()) == f->value.end())
					| (f->value.find(rightOperand->getName()) == f->value.end())) {

			} else {
				// Hmm, I guess we're good...
				float leftVal = f->value.find(leftOperand->getName())->second;

				float rightVal = f->value.find(rightOperand->getName())->second;
				float resVal = computeOp(leftVal, rightVal, opcode);

				//float resVal = leftVal + rightVal;
				ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
				//errs() << leftVal << " " << rightVal << "\n";
				//errs() << "outcome: " << resVal << "\n";
				value[K->getName()] = resVal;
				ff->value = value;
				ConstantPropAnalysisFlow* tmp =
						static_cast<ConstantPropAnalysisFlow*>(ff->join(f));
				delete ff;
				delete f;
				f = tmp;

			}

			//break;

		}

	}
	return f;
}



Flow * ConstantPropAnalysis::initialize() {
	return new ConstantPropAnalysisFlow(ConstantPropAnalysisFlow::BOTTOM);
}

ConstantPropAnalysis::ConstantPropAnalysis(Function & F) :
		StaticAnalysis() {
	this->top = new ConstantPropAnalysisFlow(ConstantPropAnalysisFlow::TOP);//Should be changed by subclasses of Flow to an instance of the subclass
	this->bottom = new ConstantPropAnalysisFlow(
			ConstantPropAnalysisFlow::BOTTOM);//Should be changed by subclasses of Flow to an instance of the subclass
	this->functionName = F.getName();
	buildCFG(F);
}
