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

#define ADD 8 //This is the opcode for the add instruction
#define FADD 9 //This is the opcode for the add instruction
#define SUB 10 //This is the opcode for the sub instruction
#define FSUB 11 //This is the opcode for the floating point sub instruction
#define MUL 12 //This is the opcode for the mul instruction
#define FMUL 13 //This is the opcode for the floating point mul instruction
#define SDIV 15 //This is the opcode for the signed div instruction
#define FDIV 16 //This is the opcode for the float div instruction

#define FPTOSI 37 //This is the opcode for the float to integer cast instruction

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
		output = executeAddInst(inFlow, inst);
		break;
	case FADD:
		output = executeFAddInst(inFlow, inst);
		break;
	case SUB:
		output = executeSubInst(inFlow, inst);
		break;
	case FSUB:
		output = executeFSubInst(inFlow, inst);
		break;
	case MUL:
		output = executeMulInst(inFlow, inst);
		break;
	case FMUL:
		output = executeFMulInst(inFlow, inst);
		break;
	case SDIV:
		output = executeSDivInst(inFlow, inst);
		break;
	case FDIV:
		output = executeFDivInst(inFlow, inst);
		break;
	case FPTOSI:
		output = executeFptosiInst(inFlow, inst);
		break;

	default:
		output = new ConstantPropAnalysisFlow(inFlow);
		break;
	}
	//errs() << "Instruction : " << *inst << ", Flow value : " << output->jsonString() << "\n";
	return output;
}

ConstantPropAnalysisFlow* ConstantPropAnalysis::executeFptosiInst(
		ConstantPropAnalysisFlow* in, Instruction* instruction) {

	ConstantPropAnalysisFlow* f = new ConstantPropAnalysisFlow(in);
	//Value *leftOperand = instruction->getOperand(0);
	//Value *rightOperand = instruction->getOperand(1);
	map<string, float> value;
	Value *retVal = instruction;
	string regName = retVal->getName();

	Value* casting = instruction->getOperand(0); //RO

	//if (!dyn_cast<ConstantFP>(retVal)) {

	//	if (!dyn_cast<ConstantFP>(casting)) {
	// Cool they are both variables. We just need to forward the value
	if (f->value.find(casting->getName()) == f->value.end()) {
		// Oh no! Read the error message!

		errs() << "Undefined variable!\n";
		errs() << "Variable: " << casting->getName() << " was not found \n";

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

	//}

	//}
	return f;

}

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
