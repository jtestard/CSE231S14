/**
 * Requirements :
 * 	-	Every static analysis must extend the StaticAnalysis class.
 * 	-	The listNode structure is used to store the results of the analysis.
 *
 * 	Notice that we assume all static analyses use a function scope, in accordance with the Professor's instructions.
 */

#ifndef STATIC_ANALYSIS
#define STATIC_ANALYSIS
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Support/raw_ostream.h"
#include "Flow.h"
#include "DomainElement.h"
#include <map>
#include <vector>
#include <cstdlib>
#include <queue>
#include <sstream>
#include <set>

using namespace llvm;
using namespace std;

//Static Analysis class
class StaticAnalysis {

public :
	//Used to build the context flow graph
	typedef struct ListNode {
		int index;
		Flow in;
		vector<ListNode*> succs;
		Instruction *inst; //uniquely identifies the node
		bool dirty;
		ListNode(int idx){
			dirty = true;
			index = idx;
		}
		~ListNode(){
			for(unsigned int i = 0 ; i < succs.size(); i++) {
				delete succs[i];
			}
		}
	} ListNode;

	//This function implements our worklist. This class should not be overwritten.
	void runWorklist();
	ListNode* getCFG();
	StringRef getFunctionName();
	void JSONCFG(raw_ostream &OS); //Returns the context graph in JSON format.
	StaticAnalysis(Function &F);
	virtual ~StaticAnalysis();

protected:
	//Would be better if those two were const static, but this is not possible in C++, so please don't change them :).
	Flow top;
	Flow bottom;

	/**
	 * This method is called by the run worklist algorithm.
	 * It has the responsability to figure out what kind of instruction is being used and how to generate the output flow from the input flow for
	 * that instruction.
	 * It is expected this function will call other functions created by the subclasses to deal with each type of instruction.
	 *
	 * WARNING : the output might not be empty to begin with. If it is not, then the output must be joined with the result of the processing
	 * of the input.
	 */
	virtual void executeFlowFunction(Flow &in, Instruction &inst, Flow &out);

private:
	void buildCFG(Function &F);
	ListNode* contextFlowGraph;
	StringRef functionName;
};
#endif
