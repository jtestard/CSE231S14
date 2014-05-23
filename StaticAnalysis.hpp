/**
 * Requirements :
 * 	-	Every static analysis must extend the StaticAnalysis class.
 * 	-	The listNode structure is used to store the results of the analysis.
 */

#ifndef STATIC_ANALYSIS
#include "llvm/IR/Module.h"
#include "Variable.h"
#include "DomainElement.h"
#include <map>
#include <vector>
#define STATIC_ANALYSIS
#endif

using namespace llvm;
using namespace std;

class StaticAnalysis {

public :
	//Used to build the context flow graph
	typedef struct lnode {
		map<Variable,vector<DomainElement> > in;
		vector<lnode> succs;
		bool dirty;
	} ListNode;

	//This function implements our worklist. This class should not be overwritten.
	void runWorklist(Module &M);
	ListNode getCFG();

protected:
	static map<Variable, vector<DomainElement> > top; // A variable may point to multiple elements of the domain
	static map<Variable, vector<DomainElement> > bottom;

private:
	void buildCFG(Module &M);
	ListNode contextFlowGraph;
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
//	//should on be called by the print(raw_ostream &OS, const Module*) from LLVM.
//	virtual void printJSON();
};
