/*
 *
 * Created by Costas Zarifis on 22/05/2014.
 *
 * Requirements :
 * 	-	Every static analysis must extend the StaticAnalysis class.
 * 	-	The listNode structure is used to store the results of the analysis.
 *
 * 	Notice that we assume all static analyses use a function scope, in accordance with the Professor's instructions.
 */

#ifndef AVAILABLE_EXPRESSION_ANALYSIS
#define AVAILABLE_EXPRESSION_ANALYSIS
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Support/raw_ostream.h"
#include "AvailableExpressionAnalysisFlow.h"
#include "StaticAnalysis.h"
#include <map>
#include <vector>
#include <cstdlib>
#include <queue>
#include <sstream>
#include <set>
#include <llvm/IR/Constants.h>
#include <time.h>
#include <stdlib.h>

using namespace llvm;
using namespace std;

//Static Analysis class
class AvailableExpressionAnalysis: public StaticAnalysis {

public:

	AvailableExpressionAnalysis(Function &F);
	/*
	 * This method is called by the run worklist algorithm.
	 * It has the responsability to figure out what kind of instruction is being used and how to generate the output flow from the input flow for
	 * that instruction.
	 * It is expected this function will call other functions created by the subclasses to deal with each type of instruction.
	 *
	 * The output is a Flow that is the result of the processing of in with respect to instruction inst.
	 */
	Flow* executeFlowFunction(Flow *in, Instruction *inst, int NodeId);

	Flow* initialize();

protected:

	AvailableExpressionAnalysisFlow *executeUnaryInst(
			AvailableExpressionAnalysisFlow* in, Instruction* inst,
			unsigned opcode);
	AvailableExpressionAnalysisFlow *executeFOpInst(
			AvailableExpressionAnalysisFlow* in, Instruction* inst,
			unsigned opcode);
	AvailableExpressionAnalysisFlow *executeOpInst(
			AvailableExpressionAnalysisFlow* in, Instruction* inst,
			unsigned opcode);
	AvailableExpressionAnalysisFlow *executePhiInst(
			AvailableExpressionAnalysisFlow* in, Instruction* inst);

private:
	string computeBinaryOp(string leftVal, string rightVal, unsigned opcode);
	string computeUnaryOp(string leftVal, unsigned opcode);
	bool isEqual(AvailableExpressionAnalysisFlow* in, Instruction *inst);
	//Variables are represented as strings.
	map<string, string> analysisMap;

};
#endif
