//
//  RangeAnalysisPass.cpp
//  
//
//  Created by Costas Zarifis on 22/05/2014.
//
//

/*
 * Note : use the errs() instead of std::cout in this file to output to the console (if your name is not mike and you don't have a fancy debugger that
 * took hours to install :).
 */
#include "RangeAnalysis.h"

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


//Flow* RangeAnalysis::executeFlowFunction(Flow *in, Instruction* inst, int index) {
//	return executeFlowFunction(Flow *in, Instruction* inst);
//}

/*
 * For basic static analysis, flow is just "assigned to top", which just means the basic string from the Flow general class will be top.
 * This method is expected to do much more when overloaded.
 */
Flow* RangeAnalysis::executeFlowFunction(Flow *in, Instruction *inst, int NodeId) {
//	errs() << "Instruction Opcode : " << inst->getOpcode() << ", get name : "
//			<< inst->getOpcodeName() << "\n";
	RangeAnalysisFlow* inFlow =
			static_cast<RangeAnalysisFlow*>(in);
	RangeAnalysisFlow * output;
//Just need to check if we have seen this basic block a few times.
//If we have seen this a few times, change ALL VARIABLES that are changing to TOP
	if(nodeCount.find(NodeId) != nodeCount.end())
	{


		if(nodeCount[NodeId].nodeVisitCounter >= 3)
		{
//ANY VARIABLES THAT DID NOT CHANGE MUST BE SET TO TOP!!!
//THIS SHOULD SET SOME VARIABLES TO TOP!!!!
			RangeAnalysisFlow* f = new RangeAnalysisFlow(inFlow);
			RangeAnalysisFlow* ff = new RangeAnalysisFlow();
			RangeAnalysisFlow* tmp = static_cast<RangeAnalysisFlow*>(ff->join(f));//Contains the value of the variable from the old set
			delete ff;
			delete f;
			f = tmp;									//Keep tmp which has the variable from.. f?

			DeleteDifferentRanges(f, nodeCount[NodeId].nodeSet);

			return(f);	//In = Out
			//Just return the in Set (no Change)
		}
		nodeCount[NodeId].nodeVisitCounter = (nodeCount[NodeId].nodeVisitCounter + 1);
	}
	else
	{
		nodeState thisNodeState;
		RangeAnalysisFlow* f = new RangeAnalysisFlow(inFlow);
		RangeAnalysisFlow* ff = new RangeAnalysisFlow();
		RangeAnalysisFlow* tmp = static_cast<RangeAnalysisFlow*>(ff->join(f));//Contains the value of the variable from the old set
		delete ff;
		delete f;
		f = tmp;

		thisNodeState.nodeVisitCounter = 1;
		thisNodeState.nodeSet = f;
		nodeCount[NodeId] = thisNodeState;	//Track this node

	}



//	errs()<< "EXECUTING FUNCTION!\n";
//	for (map<string, float>::iterator it = inFlow->value.begin();
//			it != inFlow->value.end(); it++) {
//		errs() << it->first << " -> " << it->second << "\n";
//	}


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
		output = executePhiInst(inFlow, inst);
		break;

	default:
		output = new RangeAnalysisFlow(inFlow);
		break;
	}
	//errs() << "Instruction : " << *inst << ", Flow value : " << output->jsonString() << "\n";
	return output;
}

RangeAnalysisFlow* RangeAnalysis::executeCastInst(
		RangeAnalysisFlow* in, Instruction* instruction) {

	RangeAnalysisFlow* f = new RangeAnalysisFlow(in);
	//Value *leftOperand = instruction->getOperand(0);
	//Value *rightOperand = instruction->getOperand(1);
	map<string, RangeDomainElement> value;
	Value *retVal = instruction;
	string regName = retVal->getName();

	Value* casting = instruction->getOperand(0); //RO

	if (!dyn_cast<Constant>(retVal))	//If the instruction is not a constant
	{

		if (!dyn_cast<Constant>(casting))	//If operand 0 is not a constant
		{
			// Instruction and operand 0 are both variables. We just need to forward the value
			if (f->value.find(casting->getName()) == f->value.end())
			{
				// Oh no! Read the error message!

				errs() << "Undefined variable!\n";
				errs() << "Variable: " << casting->getName()
						<< " was not found \n";

			}
			else
			{
				// Hmm, I guess we're good...
//SHOULD BE AN INTEGER TYPE....
				//Get the Range that we have for this variable
				RangeDomainElement forwardRange = f->value.find(casting->getName())->second;
				RangeAnalysisFlow* ff = new RangeAnalysisFlow();
				value[retVal->getName()] = forwardRange;		//Move the value of the stored variable into...
				ff->value = value;							//A new set
				RangeAnalysisFlow* tmp =
						static_cast<RangeAnalysisFlow*>(ff->join(f));//Contains the value of the variable from the old set
				delete ff;
				delete f;
				f = tmp;									//Keep tmp which has the variable from.. f?
			}

		}
		else 							//Operand 0 is not a constant. Is it a float?
		{

			// Hmm, I guess we're good...
			if (ConstantFP *cfp = dyn_cast<ConstantFP>(casting))
			{
				RangeDomainElement forwardRange;						//A precise range of 0 can be created using the constant.
				float forwardVal = cfp->getValueAPF().convertToFloat();

				//float forwardVal = f->value.find(casting->getName())->second;
				RangeAnalysisFlow* ff = new RangeAnalysisFlow();

				//We have a fixed value for the operand. Because of that, we have a precise range and can set the range precisely
				forwardRange.upper = forwardVal;
				forwardRange.lower = forwardVal;

				value[retVal->getName()] = forwardRange;
				ff->value = value;
				RangeAnalysisFlow* tmp =
						static_cast<RangeAnalysisFlow*>(ff->join(f));
				delete ff;
				delete f;
				f = tmp;
			}
			else if (ConstantInt *cfp = dyn_cast<ConstantInt>(casting))
			{
				RangeDomainElement forwardRange;
				float forwardVal = cfp->getZExtValue();
				RangeAnalysisFlow* ff = new RangeAnalysisFlow();


				forwardRange.upper = forwardVal;
				forwardRange.lower = forwardVal;
				value[retVal->getName()] = forwardRange;
				ff->value = value;
				RangeAnalysisFlow* tmp =
						static_cast<RangeAnalysisFlow*>(ff->join(f));
				delete ff;
				delete f;
				f = tmp;

			}

		}

	}
	return f;

}

float RangeAnalysis::computeOp(float leftVal, float rightVal,
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

//For the tricky case of range arithmetic ops
RangeDomainElement computeOpRange(RangeDomainElement leftRange, RangeDomainElement rightRange, unsigned opcode)
{
	RangeDomainElement resRange;
	float mulDivCombos[4];
	int shiftCombos[4];
	int AshiftLower, AshiftHigher, BshiftLower, BshiftHigher;
	int lowerShiftSign, upperShiftSign;
	int comboCheckCtr = 0;
//CHECK THAT BOTH ELEMENTS ARE IN RANGE!!!
	if(rightRange.undefined || leftRange.undefined || rightRange.top || leftRange.top)
	{
		resRange.top = true;	//Oh dearrrrrrrr :( This guy is not gonna be helpful anymore :(
		resRange.bottom = false;
		resRange.upper = 0;
		resRange.lower = 0;
		return resRange;
	}


	switch (opcode) {

	case ADD:
	case FADD:
		//Get the lowest of the low and the highest of the high resVal = leftVal + rightVal;
		resRange.lower = leftRange.lower + rightRange.lower;
		resRange.upper = leftRange.upper + rightRange.upper;
		break;
	case SUB:
	case FSUB:
		//Get the highest of the low and the lowest of the high. Hu-hya! resVal = leftVal - rightVal;
		resRange.lower = leftRange.lower - rightRange.lower;
		resRange.upper = leftRange.upper - rightRange.upper;
		break;
	case FDIV:
	case SDIV:
		//Combo fiend! resVal = leftVal / rightVal;
		mulDivCombos[0] = leftRange.lower / rightRange.lower;
		mulDivCombos[1] = leftRange.lower / rightRange.upper;
		mulDivCombos[2] = leftRange.upper / rightRange.lower;
		mulDivCombos[3] = leftRange.upper / rightRange.upper;
		//get the lowest of all combos for the return lower bound
		//get the highest of all combos for the return upper bound
		resRange.lower = mulDivCombos[0];	//Initialize, you must. Since we start with max in resRange.
		resRange.upper = mulDivCombos[0];
		while(comboCheckCtr < 4)
		{
			if(mulDivCombos[comboCheckCtr] < resRange.lower)
				resRange.lower = mulDivCombos[comboCheckCtr];
			if(mulDivCombos[comboCheckCtr] > resRange.upper)
				resRange.upper = mulDivCombos[comboCheckCtr];
			comboCheckCtr++;
		}
		break;
	case FMUL:
	case MUL:
//		resVal = leftVal * rightVal;
		//Combo fiend! resVal = leftVal / rightVal;
		mulDivCombos[0] = leftRange.lower * rightRange.lower;
		mulDivCombos[1] = leftRange.lower * rightRange.upper;
		mulDivCombos[2] = leftRange.upper * rightRange.lower;
		mulDivCombos[3] = leftRange.upper * rightRange.upper;
		//get the lowest of all combos for the return lower bound
		//get the highest of all combos for the return upper bound
		resRange.lower = mulDivCombos[0];	//Initialize, you must. Since we start with max in resRange.
		resRange.upper = mulDivCombos[0];
		while(comboCheckCtr < 4)
		{
			if(mulDivCombos[comboCheckCtr] < resRange.lower)
				resRange.lower = mulDivCombos[comboCheckCtr];
			if(mulDivCombos[comboCheckCtr] > resRange.upper)
				resRange.upper = mulDivCombos[comboCheckCtr];
			comboCheckCtr++;
		}
		//Add epsilon assuming rounding down. Don't want innacuracy!
//TO DO: DEBUG THIS EPSILON!
//		resRange.upper += 0.5;
		break;
	case FREM:
	case SREM:
		//resVal = (int) leftVal % (int) rightVal;
		//Combo fiend! resVal = leftVal / rightVal;
		mulDivCombos[0] = (int) leftRange.lower % (int) rightRange.lower;
		mulDivCombos[1] = (int) leftRange.lower % (int) rightRange.upper;
		mulDivCombos[2] = (int) leftRange.upper % (int) rightRange.lower;
		mulDivCombos[3] = (int) leftRange.upper % (int) rightRange.upper;
		//get the lowest of all combos for the return lower bound
		//get the highest of all combos for the return upper bound
		resRange.lower = mulDivCombos[0];	//Initialize, you must. Since we start with max in resRange.
		resRange.upper = mulDivCombos[0];
		while(comboCheckCtr < 4)
		{
			if(mulDivCombos[comboCheckCtr] < resRange.lower)
				resRange.lower = mulDivCombos[comboCheckCtr];
			if(mulDivCombos[comboCheckCtr] > resRange.upper)
				resRange.upper = mulDivCombos[comboCheckCtr];
			comboCheckCtr++;
		}
		break;
	case SHL:
//		resVal = (int) leftVal << (int) rightVal;
		//Combo fiend!
		mulDivCombos[0] = (int) leftRange.lower << (int) rightRange.lower;
		mulDivCombos[1] = (int) leftRange.lower << (int) rightRange.upper;
		mulDivCombos[2] = (int) leftRange.upper << (int) rightRange.lower;
		mulDivCombos[3] = (int) leftRange.upper << (int) rightRange.upper;
		//get the lowest of all combos for the return lower bound
		//get the highest of all combos for the return upper bound
		resRange.lower = mulDivCombos[0];	//Initialize, you must. Since we start with max in resRange.
		resRange.upper = mulDivCombos[0];
		while(comboCheckCtr < 4)
		{
			if(mulDivCombos[comboCheckCtr] < resRange.lower)
				resRange.lower = mulDivCombos[comboCheckCtr];
			if(mulDivCombos[comboCheckCtr] > resRange.upper)
				resRange.upper = mulDivCombos[comboCheckCtr];
			comboCheckCtr++;
		}
		break;
	case LSHR:
		AshiftLower = (int)leftRange.lower;
		AshiftHigher = (int)leftRange.upper;
		BshiftLower = (int)rightRange.lower;
		BshiftHigher = (int)rightRange.upper;

		shiftCombos[0] = AshiftLower >>  BshiftLower;
		shiftCombos[1] = AshiftLower >> BshiftHigher;
		shiftCombos[2] = AshiftHigher >> BshiftLower;
		shiftCombos[3] = AshiftHigher >> BshiftHigher;
		//get the lowest of all combos for the return lower bound
		//get the highest of all combos for the return upper bound
		resRange.lower = shiftCombos[0];	//Initialize, you must. Since we start with max in resRange.
		resRange.upper = shiftCombos[0];
		while(comboCheckCtr < 4)
		{
			if(shiftCombos[comboCheckCtr] < resRange.lower)
				resRange.lower = (float)shiftCombos[comboCheckCtr];
			if(shiftCombos[comboCheckCtr] > resRange.upper)
				resRange.upper = (float)shiftCombos[comboCheckCtr];
			comboCheckCtr++;
		}
		break;
	case ASHR:	//Haaaanh? TO DO: Debug.
//		resVal = (int) leftVal >> (int) rightVal;
		//another combo fiender.
		//BUBAGAWG!!!!
		AshiftLower = (int)leftRange.lower;
		AshiftHigher = (int)leftRange.upper;
		BshiftLower = (int)rightRange.lower;
		BshiftHigher = (int)rightRange.upper;
		lowerShiftSign = AshiftLower & 0x80000000;	//Save the sign bit of the left operand (low range)
		upperShiftSign = AshiftHigher & 0x80000000; //Save the sign bit of the left operand (high range)
		AshiftLower &= 0x7fffffff;					//In a copy to be shifted: Destroy the sign bit of the left operand (low range)
		AshiftLower &= 0x7fffffff;

		AshiftLower >>=  BshiftLower;
		AshiftLower |= lowerShiftSign;
		shiftCombos[0] = AshiftLower;

		AshiftLower >>= BshiftHigher;
		AshiftLower |= lowerShiftSign;
		shiftCombos[1] = AshiftLower;

		AshiftHigher >>= BshiftLower;
		AshiftHigher |= upperShiftSign;
		shiftCombos[2] = AshiftHigher;

		AshiftHigher >>= BshiftHigher;
		AshiftHigher |= BshiftHigher;
		shiftCombos[3] = AshiftHigher;
		//get the lowest of all combos for the return lower bound
		//get the highest of all combos for the return upper bound
		resRange.lower = shiftCombos[0];	//Initialize, you must. Since we start with max in resRange.
		resRange.upper = shiftCombos[0];
		while(comboCheckCtr < 4)
		{
			if(shiftCombos[comboCheckCtr] < resRange.lower)
				resRange.lower = (float)shiftCombos[comboCheckCtr];
			if(shiftCombos[comboCheckCtr] > resRange.upper)
				resRange.upper = (float)shiftCombos[comboCheckCtr];
			comboCheckCtr++;
		}
		break;
	}

	return resRange;
}




RangeAnalysisFlow* RangeAnalysis::executePhiInst(
		RangeAnalysisFlow* in, Instruction* instruction) {

	RangeAnalysisFlow* f = new RangeAnalysisFlow(in);
	Value *leftOperand = instruction->getOperand(0);
	Value *rightOperand = instruction->getOperand(1);
	map<string, RangeDomainElement> value;
	Value *K = instruction;
	string regName = K->getName();
	errs() << "Instruction : " << regName << " left " << leftOperand->getName()
			<< " right " << rightOperand->getName() << "\n";

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
		RangeDomainElement leftVal = f->value.find(leftOperand->getName())->second;

		RangeDomainElement rightVal = f->value.find(rightOperand->getName())->second;
		errs() << "leftVal: " << leftVal.upper << " , " << leftVal.lower << "rightVal:" << rightVal.upper << " , " << rightVal.lower << "\n";

/*		REPLACE THIS WITH GETTING THE MAX RANGE!!!
		// If the variables are not the same in the two branches then
		// we can't propagate the constant.
		if (leftVal == rightVal){

			float resVal = leftVal;*/
		RangeDomainElement maxRange = JoinRangeDomainElements(	(const RangeDomainElement*) &leftVal,
																(const RangeDomainElement*) &rightVal);

		RangeAnalysisFlow* ff = new RangeAnalysisFlow();
			errs() << "input" << leftVal.upper << " , " << leftVal.lower << "rightVal:" << rightVal.upper << " , " << rightVal.lower << "\n";
			errs() << "outcome: " << maxRange.upper << " , " << maxRange.lower << "\n";
			value[K->getName()] = maxRange;
			ff->value = value;
			RangeAnalysisFlow* tmp =
					static_cast<RangeAnalysisFlow*>(ff->join(f));
			delete ff;
			delete f;
			f = tmp;
		//}

	}

	return f;
}

RangeAnalysisFlow* RangeAnalysis::executeFOpInst(
		RangeAnalysisFlow* in, Instruction* instruction,
		unsigned opcode) {

	RangeAnalysisFlow* f = new RangeAnalysisFlow(in);
	Value *leftOperand = instruction->getOperand(0);
	Value *rightOperand = instruction->getOperand(1);
	map<string, RangeDomainElement> value;
	Value *K = instruction;
	string regName = K->getName();
//errs() << "Instruction : " << regName << " left " << leftOperand->getName()
//		<< " right " << rightOperand->getName() << "\n";

// Checking if left operand is a constant
	if (ConstantFP *CILeft = dyn_cast<ConstantFP>(leftOperand)) {

		if (ConstantFP *CIRight = dyn_cast<ConstantFP>(rightOperand))
		{
			RangeDomainElement resRange;
			// Cool they are both constants. you can get a precise range.


			float leftVal = CILeft->getValueAPF().convertToFloat();
			float rightVal = CIRight->getValueAPF().convertToFloat();

			float resVal = computeOp(leftVal, rightVal, opcode);
			resRange.upper = resVal;
			resRange.lower = resVal;


			RangeAnalysisFlow* ff = new RangeAnalysisFlow();
			//errs() << leftVal << " " << rightVal << "\n";
			//errs() << "outcome: " << resVal << "\n";
			value[K->getName()] = resRange;
			ff->value = value;
			RangeAnalysisFlow* tmp =
					static_cast<RangeAnalysisFlow*>(ff->join(f));
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

			else
			{
//TO DO: DEBUG THIS CODE! WORKS ON RANGES!!
				//Can still get a precise range
				RangeDomainElement leftRange, rightRange, resRange;

				// Hmm, I guess we're good...
				float leftVal = CILeft->getValueAPF().convertToFloat();
				leftRange.lower = leftVal;
				leftRange.upper = leftVal;
				rightRange = f->value.find(rightOperand->getName())->second;

				resRange = computeOpRange(leftRange, rightRange, opcode);

				RangeAnalysisFlow* ff = new RangeAnalysisFlow();
				//errs() << leftVal << " " << rightVal << "\n";
				//errs() << "outcome: " << resVal << "\n";
				value[K->getName()] = resRange;
				ff->value = value;
				RangeAnalysisFlow* tmp =
						static_cast<RangeAnalysisFlow*>(ff->join(f));
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
//DEBUG THIS!!!!
//HERE WE ARE COMPUTING OPS USING A RANGE OF VALUES, NOT THE PLAIN ABSOLUTES
				//float leftVal = f->value.find(leftOperand->getName())->second;
				//Now we are working with a range in the left hand. This will introuduce some impreciseness
				RangeDomainElement resRange, rightRange, leftRange = f->value.find(leftOperand->getName())->second;

				float rightVal = CIRight->getValueAPF().convertToFloat();
				rightRange.upper = rightVal;
				rightRange.lower = rightVal;
				//float resVal = leftVal + rightVal;

				resRange = computeOpRange(leftRange, rightRange, opcode);
				RangeAnalysisFlow* ff = new RangeAnalysisFlow();
				//errs() << leftVal << " " << rightVal << "\n";
				//errs() << "outcome: " << resVal << "\n";
				value[K->getName()] = resRange;
				ff->value = value;
				RangeAnalysisFlow* tmp =
						static_cast<RangeAnalysisFlow*>(ff->join(f));
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

			}
			else
			{
//DEBUG THIS!!!!
//HERE WE ARE COMPUTING OPS USING A RANGE OF VALUES, NOT THE PLAIN ABSOLUTES
				// Hmm, I guess we're good...
				RangeDomainElement resRange, rightRange, leftRange = f->value.find(leftOperand->getName())->second;

//				float rightVal = f->value.find(rightOperand->getName())->second;
				rightRange = f->value.find(rightOperand->getName())->second;

				resRange = computeOpRange(leftRange, rightRange, opcode);
				RangeAnalysisFlow* ff = new RangeAnalysisFlow();
				//errs() << leftVal << " " << rightVal << "\n";
				//errs() << "outcome: " << resVal << "\n";
				value[K->getName()] = resRange;
				ff->value = value;
				RangeAnalysisFlow* tmp =
						static_cast<RangeAnalysisFlow*>(ff->join(f));
				delete ff;
				delete f;
				f = tmp;

			}

		}

	}
	return f;
}

RangeAnalysisFlow* RangeAnalysis::executeOpInst(	RangeAnalysisFlow* in,
													Instruction* instruction,
													unsigned opcode				)
{
	RangeDomainElement leftRange, rightRange, resRange;
	RangeAnalysisFlow* f = new RangeAnalysisFlow(in);
	Value *leftOperand = instruction->getOperand(0);
	Value *rightOperand = instruction->getOperand(1);
	map<string, RangeDomainElement> value;
	Value *K = instruction;
	string regName = K->getName();
//errs() << "Instruction : " << regName << " left " << leftOperand->getName()
//		<< " right " << rightOperand->getName() << "\n";

// Checking if left operand is a constant
	if (ConstantInt *CILeft = dyn_cast<ConstantInt>(leftOperand)) {

		if (ConstantInt *CIRight = dyn_cast<ConstantInt>(rightOperand))
		{
			// Cool they are both constants.

			float leftVal = CILeft->getZExtValue();
			float rightVal = CIRight->getZExtValue();

			float resVal = computeOp(leftVal, rightVal, opcode);		//Get precise information
			resRange.upper = resVal;
			resRange.lower = resVal;

			//float resVal = leftVal + rightVal;
			RangeAnalysisFlow* ff = new RangeAnalysisFlow();
			//errs() << leftVal << " " << rightVal << "\n";
			//errs() << "outcome: " << resVal << "\n";
			value[K->getName()] = resRange;
			ff->value = value;
			RangeAnalysisFlow* tmp =
					static_cast<RangeAnalysisFlow*>(ff->join(f));
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
//TO DO: DEBUG THIS CODE! IT RUNS ON RANGES!!!
				// Hmm, I guess we're good...
				float leftVal = CILeft->getZExtValue();
				leftRange.lower = leftVal;
				leftRange.upper = leftVal;
				rightRange = f->value.find(rightOperand->getName())->second;
				resRange = computeOpRange(leftRange, rightRange, opcode);
				//float resVal = leftVal + rightVal;

				RangeAnalysisFlow* ff = new RangeAnalysisFlow();
				//errs() << leftVal << " " << rightVal << "\n";
				//errs() << "outcome: " << resVal << "\n";
				value[K->getName()] = resRange;
				ff->value = value;
				RangeAnalysisFlow* tmp =
						static_cast<RangeAnalysisFlow*>(ff->join(f));
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
//TO DO: DEBUG THIS CODE!!!!!
				leftRange = f->value.find(leftOperand->getName())->second;
				float rightVal = CIRight->getZExtValue();
				rightRange.upper = rightVal;
				rightRange.upper = rightVal;
				resRange = computeOpRange(leftRange, rightRange, opcode);

				//float resVal = leftVal + rightVal;
				RangeAnalysisFlow* ff = new RangeAnalysisFlow();
				//errs() << leftVal << " " << rightVal << "\n";
				//errs() << "outcome: " << resVal << "\n";
				value[K->getName()] = resRange;
				ff->value = value;
				RangeAnalysisFlow* tmp =
						static_cast<RangeAnalysisFlow*>(ff->join(f));
				delete ff;
				delete f;
				f = tmp;
			}
		}
		else
		{

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
				leftRange = f->value.find(leftOperand->getName())->second;

				rightRange = f->value.find(rightOperand->getName())->second;
				resRange = computeOpRange(leftRange, rightRange, opcode);

				//float resVal = leftVal + rightVal;
				RangeAnalysisFlow* ff = new RangeAnalysisFlow();
				//errs() << leftVal << " " << rightVal << "\n";
				//errs() << "outcome: " << resVal << "\n";
				value[K->getName()] = resRange;
				ff->value = value;
				RangeAnalysisFlow* tmp =
						static_cast<RangeAnalysisFlow*>(ff->join(f));
				delete ff;
				delete f;
				f = tmp;

			}

			//break;

		}

	}
	return f;
}


Flow * RangeAnalysis::initialize() {
	return new RangeAnalysisFlow(RangeAnalysisFlow::BOTTOM);
}

RangeAnalysis::RangeAnalysis(Function & F) :
		StaticAnalysis() {
	this->top = new RangeAnalysisFlow(RangeAnalysisFlow::TOP);//Should be changed by subclasses of Flow to an instance of the subclass
	this->bottom = new RangeAnalysisFlow(
			RangeAnalysisFlow::BOTTOM);//Should be changed by subclasses of Flow to an instance of the subclass
	this->functionName = F.getName();
	buildCFG(F);
}

//Utility function
//Delete (set to top) all variables with different ranges. This is a utility function for merging, specifically for looping
//control structures in the range analysis
void DeleteDifferentRanges(RangeAnalysisFlow* A, RangeAnalysisFlow* B)
{
	for (map<string, RangeDomainElement>::iterator it = B->value.begin();
				it != B->value.end(); it++) {

/*			if (this->value.find(it->first) == this->value.end()) {
				// They don't have the same key! We're good!
				f->value[it->first] = B->value.find(it->first)->second;
			} else {*/
		if (!(A->value.find(it->first) == A->value.end()))
		{
				// Oh no! They do have the same key! We need to check if they have
				// the same values! if they do then we're good
				RangeDomainElement thisVal = A->value.find(it->first)->second;
				RangeDomainElement BVal = B->value.find(it->first)->second;

				//if (BVal == thisVal)
				if(!RangeDomainElementisEqual(	(const RangeDomainElement*) &BVal,
												(const RangeDomainElement*) &thisVal)	)
				{
					// Both branches had different value for this variable
					//f->value[it->first] = BVal;
					thisVal.top = true;
					thisVal.bottom = false;
					thisVal.lower = 0;
					thisVal.undefined = true;
					thisVal.upper = 0;
					B->value[it->first] = thisVal;
					A->value[it->first] = thisVal;

				}
				//else
				// Nope! They have different values
				// we need to omit this variable for
				// the (implicit) "set"

			}
	}
}

//End Utility function
