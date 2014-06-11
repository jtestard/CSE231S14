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

#ifndef CONSTANT_PROP_ANALYSIS
#define CONSTANT_PROP_ANALYSIS
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Support/raw_ostream.h"
#include "ConstantPropAnalysisFlow.h"
#include "StaticAnalysis.h"
#include <map>
#include <vector>
#include <cstdlib>
#include <queue>
#include <sstream>
#include <set>
#include <llvm/IR/Constants.h>

using namespace llvm;
using namespace std;

//Static Analysis class
class ConstantPropAnalysis : public StaticAnalysis {

public :

	ConstantPropAnalysis(Function &F);
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
	ConstantPropAnalysisFlow *executeFAddInst(ConstantPropAnalysisFlow* in, Instruction* inst);
	ConstantPropAnalysisFlow *executeAddInst(ConstantPropAnalysisFlow* in, Instruction* inst);
	ConstantPropAnalysisFlow *executeFSubInst(ConstantPropAnalysisFlow* in, Instruction* inst);
	ConstantPropAnalysisFlow *executeSubInst(ConstantPropAnalysisFlow* in, Instruction* inst);
	ConstantPropAnalysisFlow *executeFMulInst(ConstantPropAnalysisFlow* in, Instruction* inst);
	ConstantPropAnalysisFlow *executeMulInst(ConstantPropAnalysisFlow* in, Instruction* inst);
	ConstantPropAnalysisFlow *executeFDivInst(ConstantPropAnalysisFlow* in, Instruction* inst);
	ConstantPropAnalysisFlow *executeSDivInst(ConstantPropAnalysisFlow* in, Instruction* inst);
	ConstantPropAnalysisFlow *executeCastInst(ConstantPropAnalysisFlow* in, Instruction* inst);

	ConstantPropAnalysisFlow *retTop();


	ConstantPropAnalysisFlow *executeFOpInst(ConstantPropAnalysisFlow* in, Instruction* inst, unsigned opcode);
	ConstantPropAnalysisFlow *executeOpInst(ConstantPropAnalysisFlow* in, Instruction* inst, unsigned opcode);
	ConstantPropAnalysisFlow *executePhiInst(ConstantPropAnalysisFlow* in, Instruction* inst);


public:
	float computeOp(float leftVal, float rightVal, unsigned opcode);





};
#endif
