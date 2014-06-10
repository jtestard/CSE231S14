//
//  AvailableExpressionAnalysisPass.cpp
//  
//
//  Created by Costas Zarifis on 22/05/2014.
//
//

/*
 * Note : use the errs() instead of std::cout in this file to output to the console (if your name is not mike and you don't have a fancy debugger that
 * took hours to install :).
 */
#include "AvailableExpressionAnalysis.h"

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
 * This method is used when a phi node is spotted. There is a possibility we are trying to merge two
 * nodes that have been casted (or any other unary operand has been applied to them) so we need to find
 * the actual initial assigment to figure out if the values are preserved
 */
bool AvailableExpressionAnalysis::isEqual(AvailableExpressionAnalysisFlow* in,
		Instruction *inst) {

	errs() << "Instruction Opcode : " << inst->getOpcode() << ", get name : "
			<< inst->getOpcodeName() << "\n";

	Value *leftOperand = inst->getOperand(0);
	Value *rightOperand = inst->getOperand(1);

	errs()<<" left: "<<leftOperand->getName()<<" right: "<<rightOperand->getName()<<"\n";
	typedef std::map<std::string, std::string>::iterator it_type;
	errs() << "I'm in isEqual\n";
	for (it_type it = in->value.begin(); it != in->value.end(); ++it) {
		string key = it->first;
		string value = it->second;

		errs() << "key: " << key << " value: " << value << "\n";
	}

	//in->value.f

//	if (in->value.find("%" + std::string(retVal->getName())){
//
//	}


	unsigned opcode = inst->getOpcode();

	switch (opcode) {

	case TRUNC:
	case ZEXT:
	case SEXT:
	case FPTOSI:
	case FPTOUI:
	case UITOFP:
	case SITOFP:
	case FPTRUNC:
	case FPEXT:
	case SREM:
		return true;
		break;

	}
	return false;

}

/*
 * For basic static analysis, flow is just "assigned to top", which just means the basic string from the Flow general class will be top.
 * This method is expected to do much more when overloaded.
 */
Flow* AvailableExpressionAnalysis::executeFlowFunction(Flow *in,
		Instruction *inst, int NodeId) {
//	errs() << "Instruction Opcode : " << inst->getOpcode() << ", get name : "
//			<< inst->getOpcodeName() << "\n";
	AvailableExpressionAnalysisFlow* inFlow =
			static_cast<AvailableExpressionAnalysisFlow*>(in);
	AvailableExpressionAnalysisFlow * output;

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
		output = executeUnaryInst(inFlow, inst, inst->getOpcode());
		break;
	case PHI:
		output = executePhiInst(inFlow, inst);
		break;

	default:
		output = new AvailableExpressionAnalysisFlow(inFlow);
		break;
	}
	//errs() << "Instruction : " << *inst << ", Flow value : " << output->jsonString() << "\n";
	return output;
}

Flow * AvailableExpressionAnalysis::initialize() {
	return new AvailableExpressionAnalysisFlow(
			AvailableExpressionAnalysisFlow::BOTTOM);
}

AvailableExpressionAnalysis::AvailableExpressionAnalysis(Function & F) :
		StaticAnalysis() {
	this->top = new AvailableExpressionAnalysisFlow(
			AvailableExpressionAnalysisFlow::TOP); //Should be changed by subclasses of Flow to an instance of the subclass
	this->bottom = new AvailableExpressionAnalysisFlow(
			AvailableExpressionAnalysisFlow::BOTTOM); //Should be changed by subclasses of Flow to an instance of the subclass
	this->functionName = F.getName();
	buildCFG(F);
	//this->analysisMap
}

AvailableExpressionAnalysisFlow* AvailableExpressionAnalysis::executeUnaryInst(
		AvailableExpressionAnalysisFlow* in, Instruction* instruction,
		unsigned opcode) {

	AvailableExpressionAnalysisFlow* f = new AvailableExpressionAnalysisFlow(
			in);
	//Value *leftOperand = instruction->getOperand(0);
	//Value *rightOperand = instruction->getOperand(1);
	map<string, string> value;
	Value *retVal = instruction;
	string regName = retVal->getName();

	Value* casting = instruction->getOperand(0); //RO

	if (!dyn_cast<Constant>(retVal)) {

		if (!dyn_cast<Constant>(casting)) {
			// Cool they are both variables. We just need to forward the value

			// Ok, cool! Both the right and the left operand is a variable...

			//string sr = rightOperand->getName();
			string resVal = computeUnaryOp(
					"%" + std::string(casting->getName()), opcode);
			if (f->value.find("%" + std::string(retVal->getName()))
					== f->value.end()) {
				AvailableExpressionAnalysisFlow* ff =
						new AvailableExpressionAnalysisFlow();
				//value[resVal] = "%" + std::string(K->getName());
				value["%" + std::string(retVal->getName())] = resVal;
				ff->value = value;
				AvailableExpressionAnalysisFlow* tmp =
						static_cast<AvailableExpressionAnalysisFlow*>(ff->join(
								f));
				delete ff;
				delete f;
				f = tmp;
			}

		} else {

			if (ConstantFP *CILeft = dyn_cast<ConstantFP>(casting)) {

				float leftVal = CILeft->getValueAPF().convertToFloat();

				std::ostringstream ss;
				ss << leftVal;
				std::string sl(ss.str());
				string resVal = computeUnaryOp(sl, opcode);

				if (f->value.find("%" + std::string(retVal->getName()))
						== f->value.end()) {

					AvailableExpressionAnalysisFlow* ff =
							new AvailableExpressionAnalysisFlow();

					//value[resVal] = "%" + std::string(K->getName());
					value["%" + std::string(retVal->getName())] = resVal;

					ff->value = value;
					AvailableExpressionAnalysisFlow* tmp =
							static_cast<AvailableExpressionAnalysisFlow*>(ff->join(
									f));
					delete ff;
					delete f;
					f = tmp;

				}
			} else if (ConstantInt *CILeft = dyn_cast<ConstantInt>(casting)) {
				int leftVal = CILeft->getZExtValue();

				std::ostringstream ss;
				ss << leftVal;
				std::string sl(ss.str());
				string resVal = computeUnaryOp(sl, opcode);

				if (f->value.find("%" + std::string(retVal->getName()))
						== f->value.end()) {

					AvailableExpressionAnalysisFlow* ff =
							new AvailableExpressionAnalysisFlow();

					//value[resVal] = "%" + std::string(K->getName());
					value["%" + std::string(retVal->getName())] = resVal;

					ff->value = value;
					AvailableExpressionAnalysisFlow* tmp =
							static_cast<AvailableExpressionAnalysisFlow*>(ff->join(
									f));
					delete ff;
					delete f;
					f = tmp;
				}

			}

		}

	}

	return f;

}

string AvailableExpressionAnalysis::computeUnaryOp(string leftVal,
		unsigned opcode) {

	string resVal = "";

	std::ostringstream ss;

	switch (opcode) {

	case TRUNC:
		ss << "trunk";
		break;
	case ZEXT:
		ss << "zext";
		break;
	case SEXT:
		ss << "sext";
		break;
	case FPTOSI:
		ss << "fptosi";
		break;
	case FPTOUI:
		ss << "fptoui";
		break;
	case UITOFP:
		ss << "uitofp";
		break;
	case SITOFP:
		ss << "sitofp";
		break;
	case FPTRUNC:
		ss << "fptrunc";
		break;
	case FPEXT:
		ss << "fpext";
		break;
	case SREM:
		ss << "srem";
		break;
	}

	ss << " ";
	ss << leftVal;
	//ss << ", ";
//ss << rightVal;
	std::string s(ss.str());

//errs() << s << "\n";
	resVal = s;
	return resVal;

}

string AvailableExpressionAnalysis::computeBinaryOp(string leftVal,
		string rightVal, unsigned opcode) {

	string resVal = "";

	std::ostringstream ss;

	switch (opcode) {

	case ADD:
		ss << "add";
		break;
	case FADD:
		ss << "fadd";
		break;
	case SUB:
		ss << "sub";
		break;
	case FSUB:
		ss << "fsub";
		break;
	case FDIV:
		ss << "fdiv";
		break;
	case SDIV:
		ss << "sdiv";
		break;
	case FMUL:
		ss << "fmul";
		break;
	case MUL:
		ss << "mul";
		break;
	case FREM:
		ss << "frem";
		break;
	case SREM:
		ss << "srem";
		break;
	case SHL:
		ss << "shl";
		break;
	case LSHR:
		ss << "lshl";
		break;
	case ASHR:
		ss << "ashr";
		break;
	}

	ss << " ";
	ss << leftVal;
	ss << ", ";
	ss << rightVal;
	std::string s(ss.str());

//errs() << s << "\n";
	resVal = s;
	return resVal;

}

AvailableExpressionAnalysisFlow* AvailableExpressionAnalysis::executePhiInst(
		AvailableExpressionAnalysisFlow* in, Instruction* instruction) {

	AvailableExpressionAnalysisFlow* f = new AvailableExpressionAnalysisFlow(
			in);
	Value *leftOperand = instruction->getOperand(0);
	Value *rightOperand = instruction->getOperand(1);
	map<string, string> value;
	Value *K = instruction;
	string regName = K->getName();
	errs() << "Instruction : " << regName << " left " << leftOperand->getName()
			<< " right " << rightOperand->getName() << "\n";

	// Ok, cool! Both the right and the left operand is a variable...
	if ((f->value.find("%" + std::string(leftOperand->getName()))
			== f->value.end())
			| (f->value.find("%" + std::string(rightOperand->getName()))
					== f->value.end())) {
		// Oh no! Read the error message!

		errs()
				<< "No information was detected for one or both of the operators!\n";

	} else {
		// Hmm, I guess we're good...
		string leftVal = f->value.find(
				"%" + std::string(leftOperand->getName()))->second;

		string rightVal = f->value.find(
				"%" + std::string(rightOperand->getName()))->second;
		errs() << "leftVal: " << leftVal << "rightVal" << rightVal << "\n";

		// If the variables are not the same in the two branches then
		// we can't propagate the constant.

		this->isEqual(in, instruction);
		if (leftVal == rightVal) {

			string resVal = leftVal;
			AvailableExpressionAnalysisFlow* ff =
					new AvailableExpressionAnalysisFlow();
			errs() << leftVal << " " << rightVal << "\n";
			errs() << "outcome: " << resVal << "\n";
			value["%" + std::string(K->getName())] = resVal;
			ff->value = value;
			AvailableExpressionAnalysisFlow* tmp =
					static_cast<AvailableExpressionAnalysisFlow*>(ff->join(f));
			delete ff;
			delete f;
			f = tmp;
		}

	}
	return f;
}

AvailableExpressionAnalysisFlow* AvailableExpressionAnalysis::executeFOpInst(
		AvailableExpressionAnalysisFlow* in, Instruction* instruction,
		unsigned opcode) {

	AvailableExpressionAnalysisFlow* f = new AvailableExpressionAnalysisFlow(
			in);
	Value *leftOperand = instruction->getOperand(0);
	Value *rightOperand = instruction->getOperand(1);
	map<string, string> value;
	Value *K = instruction;
	string regName = K->getName();
//errs() << "Instruction : " << regName << " left " << leftOperand->getName()
//		<< " right " << rightOperand->getName() << "\n";

// Checking if left operand is a constant
	if (ConstantFP *CILeft = dyn_cast<ConstantFP>(leftOperand)) {

		float leftVal = CILeft->getValueAPF().convertToFloat();
		if (ConstantFP *CIRight = dyn_cast<ConstantFP>(rightOperand)) {
			// Cool they are both constants.

			float rightVal = CIRight->getValueAPF().convertToFloat();

			std::ostringstream ss;
			ss << leftVal;
			std::string sl(ss.str());

			std::ostringstream ssr;
			ssr << rightVal;
			std::string sr(ssr.str());

			string resVal = computeBinaryOp(sl, sr, opcode);

			if (f->value.find("%" + std::string(K->getName()))
					== f->value.end()) {

				AvailableExpressionAnalysisFlow* ff =
						new AvailableExpressionAnalysisFlow();

				//value[resVal] = "%" + std::string(K->getName());
				value["%" + std::string(K->getName())] = resVal;

				ff->value = value;
				AvailableExpressionAnalysisFlow* tmp =
						static_cast<AvailableExpressionAnalysisFlow*>(ff->join(
								f));
				delete ff;
				delete f;
				f = tmp;
			}
		} else {

			std::ostringstream ss;
			ss << leftVal;
			std::string sl(ss.str());
			string sr = rightOperand->getName();

			string resVal = computeBinaryOp(sl, "%" + sr, opcode);

			if (f->value.find("%" + std::string(K->getName()))
					== f->value.end()) {

				AvailableExpressionAnalysisFlow* ff =
						new AvailableExpressionAnalysisFlow();
				//value[resVal] = "%" + std::string(K->getName());
				value["%" + std::string(K->getName())] = resVal;
				ff->value = value;
				AvailableExpressionAnalysisFlow* tmp =
						static_cast<AvailableExpressionAnalysisFlow*>(ff->join(
								f));
				delete ff;
				delete f;
				f = tmp;
			}
		}
	} else {
		// So, the left part of the addition is a variable. We'll have to check the input set to get the value
		// this variable has at the moment.

		string sl = leftOperand->getName();

		if (ConstantFP *CIRight = dyn_cast<ConstantFP>(rightOperand)) {
			// Ok, cool! the right part is a constant...

			float rightVal = CIRight->getValueAPF().convertToFloat();

			std::ostringstream ssr;
			ssr << rightVal;
			std::string sr(ssr.str());

			string resVal = computeBinaryOp("%" + sl, sr, opcode);
			if (f->value.find("%" + std::string(K->getName()))
					== f->value.end()) {
				AvailableExpressionAnalysisFlow* ff =
						new AvailableExpressionAnalysisFlow();
				//value[resVal] = "%" + std::string(K->getName());
				value["%" + std::string(K->getName())] = resVal;
				ff->value = value;
				AvailableExpressionAnalysisFlow* tmp =
						static_cast<AvailableExpressionAnalysisFlow*>(ff->join(
								f));
				delete ff;
				delete f;
				f = tmp;
			}

			//leftOperand->getName()

			//int leftVal = CILeft->getZExtValue();

		} else {

			// Ok, cool! Both the right and the left operand is a variable...

			string sr = rightOperand->getName();
			string resVal = computeBinaryOp("%" + sl, "%" + sr, opcode);
			if (f->value.find("%" + std::string(K->getName()))
					== f->value.end()) {
				AvailableExpressionAnalysisFlow* ff =
						new AvailableExpressionAnalysisFlow();
				//value[resVal] = "%" + std::string(K->getName());
				value["%" + std::string(K->getName())] = resVal;
				ff->value = value;
				AvailableExpressionAnalysisFlow* tmp =
						static_cast<AvailableExpressionAnalysisFlow*>(ff->join(
								f));
				delete ff;
				delete f;
				f = tmp;
			}

		}

	}
	return f;
}

AvailableExpressionAnalysisFlow* AvailableExpressionAnalysis::executeOpInst(
		AvailableExpressionAnalysisFlow* in, Instruction* instruction,
		unsigned opcode) {

	AvailableExpressionAnalysisFlow* f = new AvailableExpressionAnalysisFlow(
			in);
	Value *leftOperand = instruction->getOperand(0);
	Value *rightOperand = instruction->getOperand(1);
	map<string, string> value;
	Value *K = instruction;
	string regName = K->getName();
//errs() << "Instruction : " << regName << " left " << leftOperand->getName()
//		<< " right " << rightOperand->getName() << "\n";

// Checking if left operand is a constant
	if (ConstantInt *CILeft = dyn_cast<ConstantInt>(leftOperand)) {

		float leftVal = CILeft->getZExtValue();
		if (ConstantInt *CIRight = dyn_cast<ConstantInt>(rightOperand)) {
			// Cool they are both constants.

			float rightVal = CIRight->getZExtValue();

			std::ostringstream ss;
			ss << leftVal;
			std::string sl(ss.str());

			std::ostringstream ssr;
			ssr << rightVal;
			std::string sr(ssr.str());

			string resVal = computeBinaryOp(sl, sr, opcode);
			if (f->value.find("%" + std::string(K->getName()))
					== f->value.end()) {

				AvailableExpressionAnalysisFlow* ff =
						new AvailableExpressionAnalysisFlow();
				//errs() << leftVal << " " << rightVal << "\n";
				//errs() << "outcome: " << resVal << "\n";
				//value[resVal] = "%" + std::string(K->getName());
				value["%" + std::string(K->getName())] = resVal;
				//			std::ostringstream ss;
				//			ss << value;
				//			std::string s(ss.str());
				ff->value = value;
				AvailableExpressionAnalysisFlow* tmp =
						static_cast<AvailableExpressionAnalysisFlow*>(ff->join(
								f));
				delete ff;
				delete f;
				f = tmp;
			}
		} else {

			std::ostringstream ss;
			ss << leftVal;
			std::string sl(ss.str());
			string sr = rightOperand->getName();
			string resVal = computeBinaryOp(sl, "%" + sr, opcode);
			if (f->value.find("%" + std::string(K->getName()))
					== f->value.end()) {
				AvailableExpressionAnalysisFlow* ff =
						new AvailableExpressionAnalysisFlow();
				//value[resVal] = "%" + std::string(K->getName());
				value["%" + std::string(K->getName())] = resVal;
				ff->value = value;
				AvailableExpressionAnalysisFlow* tmp =
						static_cast<AvailableExpressionAnalysisFlow*>(ff->join(
								f));
				delete ff;
				delete f;
				f = tmp;
			}
		}
	} else {
		// So, the left part of the addition is a variable. We'll have to check the input set to get the value
		// this variable has at the moment.

		string sl = leftOperand->getName();

		if (ConstantInt *CIRight = dyn_cast<ConstantInt>(rightOperand)) {
			// Ok, cool! the right part is a constant...

			float rightVal = CIRight->getZExtValue();

			std::ostringstream ssr;
			ssr << rightVal;
			std::string sr(ssr.str());

			string resVal = computeBinaryOp("%" + sl, sr, opcode);
			if (f->value.find("%" + std::string(K->getName()))
					== f->value.end()) {

				AvailableExpressionAnalysisFlow* ff =
						new AvailableExpressionAnalysisFlow();
				//value[resVal] = "%" + std::string(K->getName());
				value["%" + std::string(K->getName())] = resVal;
				ff->value = value;
				AvailableExpressionAnalysisFlow* tmp =
						static_cast<AvailableExpressionAnalysisFlow*>(ff->join(
								f));
				delete ff;
				delete f;
				f = tmp;
			}

			//leftOperand->getName()

			//int leftVal = CILeft->getZExtValue();

		} else {

			// Ok, cool! Both the right and the left operand is a variable...

			string sr = rightOperand->getName();
			string resVal = computeBinaryOp("%" + sl, "%" + sr, opcode);
			if (f->value.find("%" + std::string(K->getName()))
					== f->value.end()) {

				AvailableExpressionAnalysisFlow* ff =
						new AvailableExpressionAnalysisFlow();
				//value[resVal] = "%" + std::string(K->getName());
				value["%" + std::string(K->getName())] = resVal;
				ff->value = value;
				AvailableExpressionAnalysisFlow* tmp =
						static_cast<AvailableExpressionAnalysisFlow*>(ff->join(
								f));
				delete ff;
				delete f;
				f = tmp;
			}

		}

	}
	return f;
}

