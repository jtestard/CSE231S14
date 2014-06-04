//
//  StaticAnalysisPass.cpp
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


// Logical operators (integer operands)
/*122	HANDLE_BINARY_INST(20, Shl  , BinaryOperator) // Shift left  (logical)
123	HANDLE_BINARY_INST(21, LShr , BinaryOperator) // Shift right (logical)
124	HANDLE_BINARY_INST(22, AShr , BinaryOperator) // Shift right (arithmetic)
125	HANDLE_BINARY_INST(23, And  , BinaryOperator)
126	HANDLE_BINARY_INST(24, Or   , BinaryOperator)
127	HANDLE_BINARY_INST(25, Xor  , BinaryOperator)
128	  LAST_BINARY_INST(25)
*/

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
Flow* ConstantPropAnalysis::executeFlowFunction(Flow *in, Instruction* inst) {
	errs() << "Instruction Opcode : " << inst->getOpcode() << ", get name : "
			<< inst->getOpcodeName() << "\n";
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
		//output = executeAddInst(inFlow, inst);
		output = executeOpInst(inFlow, inst, inst->getOpcode());
		break;
	case FADD:
	case FSUB:
	case FMUL:
	case FDIV:
	case FREM:
		//output = executeFDivInst(inFlow, inst);
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
		//errs() << "HOLLY FUCK WE'RE SCREWED!";
		//break;

	default:
		output = new ConstantPropAnalysisFlow(inFlow);
		break;
	}
	//errs() << "Instruction : " << *inst << ", Flow value : " << output->jsonString() << "\n";
	return output;
}

ConstantPropAnalysisFlow* ConstantPropAnalysis::executeCastInst(
		ConstantPropAnalysisFlow* in, Instruction* instruction) {

	ConstantPropAnalysisFlow* f = new ConstantPropAnalysisFlow(in);
	//Value *leftOperand = instruction->getOperand(0);
	//Value *rightOperand = instruction->getOperand(1);
	map<string, float> value;
	Value *retVal = instruction;
	string regName = retVal->getName();

	Value* casting = instruction->getOperand(0); //RO

	if (!dyn_cast<Constant>(retVal)) {

		if (!dyn_cast<Constant>(casting)) {
			// Cool they are both variables. We just need to forward the value
			if (f->value.find(casting->getName()) == f->value.end()) {
				// Oh no! Read the error message!

				errs() << "Undefined variable!\n";
				errs() << "Variable: " << casting->getName()
						<< " was not found \n";

			} else {
				// Hmm, I guess we're good...

				float forwardVal = f->value.find(casting->getName())->second;
				ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
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
		resVal = (int)leftVal <<  (int)rightVal;
		break;
	case LSHR:
	case ASHR:
		resVal = (int)leftVal >>  (int)rightVal;
		break;
	}

	return resVal;

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
//errs() << "Instruction : " << regName << " left " << leftOperand->getName()
//		<< " right " << rightOperand->getName() << "\n";

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
				errs() << "Oh no! Something went wrong!\n";
				errs() << "Undefined variable!\n";
				errs() << "Apparently the right operand of the op is";
				errs() << " a variable but this is the first time we ";
				errs() << "come across this variable!!\n";
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
				// Oh no! Read the error message!
				errs() << "Oh no! Something went terribly wrong!\n";
				errs() << "Undefined variable!\n";
				errs() << "Apparently the left operand of the op is";
				errs() << " a variable but this is the first time we ";
				errs() << "come across this variable!!\n";

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
				// Oh no! Read the error message!
				errs() << "Oh no! Something went terribly wrong!\n";
				errs() << "Undefined variable!\n";
				errs() << "Apparently the left operand of the op is";
				errs() << " a variable but this is the first time we ";
				errs() << "come across this variable!!\n";

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
				// Oh no! Read the error message!
				errs() << "Oh no! Something went wrong!\n";
				errs() << "Undefined variable!\n";
				errs() << "Apparently the right operand of the op is";
				errs() << " a variable but this is the first time we ";
				errs() << "come across this variable!!\n";

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
				// Oh no! Read the error message!
				errs() << "Oh no! Something went terribly wrong!\n";
				errs() << "Undefined variable!\n";
				errs() << "Apparently the left operand of the op is";
				errs() << " a variable but this is the first time we ";
				errs() << "come across this variable!!\n";

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
				// Oh no! Read the error message!
				errs() << "Oh no! Something went terribly wrong!\n";
				errs() << "Undefined variable!\n";
				errs() << "Apparently the left operand of the op is";
				errs() << " a variable but this is the first time we ";
				errs() << "come across this variable!!\n";

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

/*
 ConstantPropAnalysisFlow* ConstantPropAnalysis::executeFAddInst(
 ConstantPropAnalysisFlow* in, Instruction* instruction) {

 ConstantPropAnalysisFlow* f = new ConstantPropAnalysisFlow(in);
 Value *leftOperand = instruction->getOperand(0);
 Value *rightOperand = instruction->getOperand(1);
 map<string, float> value;
 Value *K = instruction;
 string regName = K->getName();
 //errs() << "Instruction : " << regName << " left " << leftOperand->getName()
 //		<< " right " << rightOperand->getName() << "\n";

 // Checking if left operand is a constant
 if (ConstantFP *CILeft = dyn_cast<ConstantFP>(leftOperand)) {

 if (ConstantFP *CIRight = dyn_cast<ConstantFP>(rightOperand)) {
 // Cool they are both constants.

 float leftVal = CILeft->getValueAPF().convertToFloat();
 float rightVal = CIRight->getValueAPF().convertToFloat();
 float resVal = leftVal + rightVal;
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
 errs() << "Oh no! Something went wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the right operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";
 }

 else {

 // Hmm, I guess we're good...
 float leftVal = CILeft->getValueAPF().convertToFloat();
 float rightVal = f->value.find(rightOperand->getName())->second;
 float resVal = leftVal + rightVal;

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
 // Oh no! Read the error message!
 errs() << "Oh no! Something went terribly wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the left operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";

 } else {
 // Hmm, I guess we're good...

 float leftVal = f->value.find(leftOperand->getName())->second;
 float rightVal = CIRight->getValueAPF().convertToFloat();
 float resVal = leftVal + rightVal;
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
 // Oh no! Read the error message!
 errs() << "Oh no! Something went terribly wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the left operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";

 } else {
 // Hmm, I guess we're good...
 float leftVal = f->value.find(leftOperand->getName())->second;

 float rightVal = f->value.find(rightOperand->getName())->second;
 float resVal = leftVal + rightVal;
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

 ConstantPropAnalysisFlow* ConstantPropAnalysis::executeAddInst(
 ConstantPropAnalysisFlow* in, Instruction* instruction) {

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
 float resVal = leftVal + rightVal;
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
 errs() << "Oh no! Something went wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the right operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";

 }

 else {

 // Hmm, I guess we're good...
 float leftVal = CILeft->getZExtValue();
 float rightVal = f->value.find(rightOperand->getName())->second;
 float resVal = leftVal + rightVal;

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
 // Oh no! Read the error message!
 errs() << "Oh no! Something went terribly wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the left operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";

 } else {
 // Hmm, I guess we're good...

 float leftVal = f->value.find(leftOperand->getName())->second;
 float rightVal = CIRight->getZExtValue();
 float resVal = leftVal + rightVal;
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
 // Oh no! Read the error message!
 errs() << "Oh no! Something went terribly wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the left operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";

 } else {
 // Hmm, I guess we're good...
 float leftVal = f->value.find(leftOperand->getName())->second;

 float rightVal = f->value.find(rightOperand->getName())->second;
 float resVal = leftVal + rightVal;
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

 ConstantPropAnalysisFlow* ConstantPropAnalysis::executeFDivInst(
 ConstantPropAnalysisFlow* in, Instruction* instruction) {

 ConstantPropAnalysisFlow* f = new ConstantPropAnalysisFlow(in);
 Value *leftOperand = instruction->getOperand(0);
 Value *rightOperand = instruction->getOperand(1);
 map<string, float> value;
 Value *K = instruction;
 string regName = K->getName();

 if (ConstantFP *CILeft = dyn_cast<ConstantFP>(leftOperand)) {

 if (ConstantFP *CIRight = dyn_cast<ConstantFP>(rightOperand)) {
 // Cool they are both constants.

 float leftVal = CILeft->getValueAPF().convertToFloat();
 float rightVal = CIRight->getValueAPF().convertToFloat();
 float resVal = leftVal / rightVal;
 ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();

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
 errs() << "Oh no! Something went wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the right operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";
 }

 else {

 // Hmm, I guess we're good...
 float leftVal = CILeft->getValueAPF().convertToFloat();
 float rightVal = f->value.find(rightOperand->getName())->second;
 float resVal = leftVal / rightVal;

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
 } else {
 // So, the left part of the addition is a variable. We'll have to check the input set to get the value
 // this variable has at the moment.
 if (ConstantFP *CIRight = dyn_cast<ConstantFP>(rightOperand)) {
 // Ok, cool! the right part is a constant...

 if (f->value.find(leftOperand->getName()) == f->value.end()) {
 // Oh no! Read the error message!
 errs() << "Oh no! Something went terribly wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the left operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";

 } else {
 // Hmm, I guess we're good...

 float leftVal = f->value.find(leftOperand->getName())->second;
 float rightVal = CIRight->getValueAPF().convertToFloat();
 float resVal = leftVal / rightVal;
 ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
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
 // Oh no! Read the error message!
 errs() << "Oh no! Something went terribly wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the left operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";

 } else {
 // Hmm, I guess we're good...
 float leftVal = f->value.find(leftOperand->getName())->second;

 float rightVal = f->value.find(rightOperand->getName())->second;
 float resVal = leftVal / rightVal;
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

 ConstantPropAnalysisFlow* ConstantPropAnalysis::executeSDivInst(
 ConstantPropAnalysisFlow* in, Instruction* instruction) {

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
 float resVal = leftVal / rightVal;
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
 errs() << "Oh no! Something went wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the right operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";
 ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
 ConstantPropAnalysisFlow* tmp =
 static_cast<ConstantPropAnalysisFlow*>(ff->join(f));

 delete ff;
 delete f;
 f = tmp;
 }

 else {

 // Hmm, I guess we're good...
 float leftVal = CILeft->getZExtValue();
 float rightVal = f->value.find(rightOperand->getName())->second;
 float resVal = leftVal / rightVal;

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
 // Oh no! Read the error message!
 errs() << "Oh no! Something went terribly wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the left operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";

 ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
 ConstantPropAnalysisFlow* tmp =
 static_cast<ConstantPropAnalysisFlow*>(ff->join(f));
 delete ff;
 delete f;
 f = tmp;
 } else {
 // Hmm, I guess we're good...

 float leftVal = f->value.find(leftOperand->getName())->second;
 float rightVal = CIRight->getZExtValue();
 float resVal = leftVal / rightVal;
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
 // Oh no! Read the error message!
 errs() << "Oh no! Something went terribly wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the left operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";
 ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
 ConstantPropAnalysisFlow* tmp =
 static_cast<ConstantPropAnalysisFlow*>(ff->join(f));
 delete ff;
 delete f;
 f = tmp;

 } else {
 // Hmm, I guess we're good...
 float leftVal = f->value.find(leftOperand->getName())->second;

 float rightVal = f->value.find(rightOperand->getName())->second;
 float resVal = leftVal / rightVal;
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

 ConstantPropAnalysisFlow* ConstantPropAnalysis::executeFMulInst(
 ConstantPropAnalysisFlow* in, Instruction* instruction) {

 ConstantPropAnalysisFlow* f = new ConstantPropAnalysisFlow(in);
 Value *leftOperand = instruction->getOperand(0);
 Value *rightOperand = instruction->getOperand(1);
 map<string, float> value;
 Value *K = instruction;
 string regName = K->getName();

 if (ConstantFP *CILeft = dyn_cast<ConstantFP>(leftOperand)) {

 if (ConstantFP *CIRight = dyn_cast<ConstantFP>(rightOperand)) {
 // Cool they are both constants.

 float leftVal = CILeft->getValueAPF().convertToFloat();
 float rightVal = CIRight->getValueAPF().convertToFloat();
 float resVal = leftVal * rightVal;
 ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();

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
 errs() << "Oh no! Something went wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the right operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";
 }

 else {

 // Hmm, I guess we're good...
 float leftVal = CILeft->getValueAPF().convertToFloat();
 float rightVal = f->value.find(rightOperand->getName())->second;
 float resVal = leftVal * rightVal;

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
 } else {
 // So, the left part of the addition is a variable. We'll have to check the input set to get the value
 // this variable has at the moment.
 if (ConstantFP *CIRight = dyn_cast<ConstantFP>(rightOperand)) {
 // Ok, cool! the right part is a constant...

 if (f->value.find(leftOperand->getName()) == f->value.end()) {
 // Oh no! Read the error message!
 errs() << "Oh no! Something went terribly wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the left operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";

 } else {
 // Hmm, I guess we're good...

 float leftVal = f->value.find(leftOperand->getName())->second;
 float rightVal = CIRight->getValueAPF().convertToFloat();
 float resVal = leftVal * rightVal;
 ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
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
 // Oh no! Read the error message!
 errs() << "Oh no! Something went terribly wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the left operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";

 } else {
 // Hmm, I guess we're good...
 float leftVal = f->value.find(leftOperand->getName())->second;

 float rightVal = f->value.find(rightOperand->getName())->second;
 float resVal = leftVal * rightVal;
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

 ConstantPropAnalysisFlow* ConstantPropAnalysis::executeMulInst(
 ConstantPropAnalysisFlow* in, Instruction* instruction) {

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
 float resVal = leftVal * rightVal;
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
 errs() << "Oh no! Something went wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the right operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";
 ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
 ConstantPropAnalysisFlow* tmp =
 static_cast<ConstantPropAnalysisFlow*>(ff->join(f));

 delete ff;
 delete f;
 f = tmp;
 }

 else {

 // Hmm, I guess we're good...
 float leftVal = CILeft->getZExtValue();
 float rightVal = f->value.find(rightOperand->getName())->second;
 float resVal = leftVal * rightVal;

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
 // Oh no! Read the error message!
 errs() << "Oh no! Something went terribly wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the left operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";

 ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
 ConstantPropAnalysisFlow* tmp =
 static_cast<ConstantPropAnalysisFlow*>(ff->join(f));
 delete ff;
 delete f;
 f = tmp;
 } else {
 // Hmm, I guess we're good...

 float leftVal = f->value.find(leftOperand->getName())->second;
 float rightVal = CIRight->getZExtValue();
 float resVal = leftVal * rightVal;
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
 // Oh no! Read the error message!
 errs() << "Oh no! Something went terribly wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the left operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";
 ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
 ConstantPropAnalysisFlow* tmp =
 static_cast<ConstantPropAnalysisFlow*>(ff->join(f));
 delete ff;
 delete f;
 f = tmp;

 } else {
 // Hmm, I guess we're good...
 float leftVal = f->value.find(leftOperand->getName())->second;

 float rightVal = f->value.find(rightOperand->getName())->second;
 float resVal = leftVal * rightVal;
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

 ConstantPropAnalysisFlow* ConstantPropAnalysis::executeFSubInst(
 ConstantPropAnalysisFlow* in, Instruction* instruction) {

 ConstantPropAnalysisFlow* f = new ConstantPropAnalysisFlow(in);
 Value *leftOperand = instruction->getOperand(0);
 Value *rightOperand = instruction->getOperand(1);
 map<string, float> value;
 Value *K = instruction;
 string regName = K->getName();

 if (ConstantFP *CILeft = dyn_cast<ConstantFP>(leftOperand)) {

 if (ConstantFP *CIRight = dyn_cast<ConstantFP>(rightOperand)) {
 // Cool they are both constants.

 float leftVal = CILeft->getValueAPF().convertToFloat();
 float rightVal = CIRight->getValueAPF().convertToFloat();
 float resVal = leftVal - rightVal;
 ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();

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
 errs() << "Oh no! Something went wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the right operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";
 }

 else {

 // Hmm, I guess we're good...
 float leftVal = CILeft->getValueAPF().convertToFloat();
 float rightVal = f->value.find(rightOperand->getName())->second;
 float resVal = leftVal - rightVal;

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
 } else {
 // So, the left part of the addition is a variable. We'll have to check the input set to get the value
 // this variable has at the moment.
 if (ConstantFP *CIRight = dyn_cast<ConstantFP>(rightOperand)) {
 // Ok, cool! the right part is a constant...

 if (f->value.find(leftOperand->getName()) == f->value.end()) {
 // Oh no! Read the error message!
 errs() << "Oh no! Something went terribly wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the left operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";

 } else {
 // Hmm, I guess we're good...

 float leftVal = f->value.find(leftOperand->getName())->second;
 float rightVal = CIRight->getValueAPF().convertToFloat();
 float resVal = leftVal - rightVal;
 ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
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
 // Oh no! Read the error message!
 errs() << "Oh no! Something went terribly wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the left operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";

 } else {
 // Hmm, I guess we're good...
 float leftVal = f->value.find(leftOperand->getName())->second;

 float rightVal = f->value.find(rightOperand->getName())->second;
 float resVal = leftVal - rightVal;
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

 ConstantPropAnalysisFlow* ConstantPropAnalysis::executeSubInst(
 ConstantPropAnalysisFlow* in, Instruction* instruction) {

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
 float resVal = leftVal - rightVal;
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
 errs() << "Oh no! Something went wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the right operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";
 ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
 ConstantPropAnalysisFlow* tmp =
 static_cast<ConstantPropAnalysisFlow*>(ff->join(f));

 delete ff;
 delete f;
 f = tmp;
 }

 else {

 // Hmm, I guess we're good...
 float leftVal = CILeft->getZExtValue();
 float rightVal = f->value.find(rightOperand->getName())->second;
 float resVal = leftVal - rightVal;

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
 // Oh no! Read the error message!
 errs() << "Oh no! Something went terribly wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the left operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";

 ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
 ConstantPropAnalysisFlow* tmp =
 static_cast<ConstantPropAnalysisFlow*>(ff->join(f));
 delete ff;
 delete f;
 f = tmp;
 } else {
 // Hmm, I guess we're good...

 float leftVal = f->value.find(leftOperand->getName())->second;
 float rightVal = CIRight->getZExtValue();
 float resVal = leftVal - rightVal;
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
 // Oh no! Read the error message!
 errs() << "Oh no! Something went terribly wrong!\n";
 errs() << "Undefined variable!\n";
 errs() << "Apparently the left operand of the op is";
 errs() << " a variable but this is the first time we ";
 errs() << "come across this variable!!\n";
 ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
 ConstantPropAnalysisFlow* tmp =
 static_cast<ConstantPropAnalysisFlow*>(ff->join(f));
 delete ff;
 delete f;
 f = tmp;

 } else {
 // Hmm, I guess we're good...
 float leftVal = f->value.find(leftOperand->getName())->second;

 float rightVal = f->value.find(rightOperand->getName())->second;
 float resVal = leftVal - rightVal;
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

 errs() << "something is wrong 2\n";
 }

 }
 return f;
 }
 */

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
