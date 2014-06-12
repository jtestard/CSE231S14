/*
 *
 * Requirements :
 * 	-	Every static analysis must extend the StaticAnalysis class.
 * 	-	The listNode structure is used to store the results of the analysis.
 *
 * 	Notice that we assume all static analyses use a function scope, in accordance with the Professor's instructions.
 */

#ifndef POINTER_ANALYSIS
#define POINTER_ANALYSIS
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"
#include "PointerAnalysisFlow.h"
#include "StaticAnalysis.h"
#include <map>
#include <vector>
#include <cstdlib>
#include <queue>
#include <sstream>
#include <set>

using namespace llvm;
using namespace std;

//Static Analysis class
class PointerAnalysis : public StaticAnalysis {

public :

	PointerAnalysis(Function &F);

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
	PointerAnalysisFlow* execute_X_equals_refY(PointerAnalysisFlow* in, Instruction* inst);
	PointerAnalysisFlow* execute_X_equals_Y(PointerAnalysisFlow* in, Instruction* inst);
	PointerAnalysisFlow* execute_ptrX_equals_Y(PointerAnalysisFlow* in, Instruction* inst);
	PointerAnalysisFlow* execute_X_equals_ptrY(PointerAnalysisFlow* in, Instruction* inst);
	PointerAnalysisFlow* execute_X_equals_NULL(PointerAnalysisFlow* in, Instruction* inst);
	bool madeByLLVM(string name);
};
#endif
