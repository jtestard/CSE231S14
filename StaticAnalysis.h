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
#include "Variable.h"
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
		map<Variable,vector<DomainElement> > out;
		vector<ListNode*> succs;
		Instruction *inst; //uniquely identifies the node
		bool dirty;
		ListNode(int idx){
			dirty = true;
			index = idx;
		}
		//Not sure if we need a destructor
	} ListNode;

	//This function implements our worklist. This class should not be overwritten.
	void runWorklist();
	ListNode* getCFG();
	StringRef getFunctionName();
	void JSONCFG(raw_ostream &OS); //Returns the context graph in JSON format.
	StaticAnalysis(Function &F);
	~StaticAnalysis();

protected:
	static map<Variable, vector<DomainElement> > top; // A variable may point to multiple elements of the domain
	static map<Variable, vector<DomainElement> > bottom;

private:
	void buildCFG(Function &F);
	ListNode* contextFlowGraph;
	StringRef functionName;
//
//	/**
//	 *  bbeval returns true if the basic block (listnode) is dirty (child node should be pushed on the worklist).
//	 *  bbeval operates on the basic block level and executes all flow functions within the basic block.
//	 */
//	virtual bool bbeval(ListNode &listNode,BasicBlock &bb) = 0;
//	virtual void flowFunction(map<Variable, vector<T>> &in, Instruction &inst, map<Variable, vector<T>> &out) = 0;
//	virtual void join(ListNode &ln1, ListNode &ln2) = 0;
//	virtual void split(ListNode &ln) = 0; //unsure if needed.
//
//	//Flow output for each instruction should be printable in JSON. This function
//	//should on be called by the print(raw_ostream &OS, const Function*) from LLVM.
//	virtual void printJSON();
};
#endif
