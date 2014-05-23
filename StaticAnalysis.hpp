/**
 * Requirements :
 * 	-	Every static analysis must extend the StaticAnalysis class.
 * 	-	The template T correspond to an element of the domain defined by the client (examples) :
 * 		-	Intra Procedural Pointer Analysis : a variable
 * 		-	Range Analysis : a tuple (min,max) of integers
 */

//#define Setvalue map<Variable,vector<T>>
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include "Variable.h"
#include <map>
#include <vector>

using namespace llvm;
using namespace std;

class StaticAnalysis/*<T>*/ {

//	//Should be made a class.
//	typedef struct lnode {
//		map<Variable,vector<T>> in;
//		BasicBlock * bb;
//		bool dirty;
//	} ListNode;
//
//	static char ID;
//	virtual static map<Variable, vector<T>> top; // A variable may point to multiple elements of the domain
//	virtual static map<Variable, vector<T>> bottom;
//
//	//This function implements our worklist
//	virtual void runWorklist(Module &M);
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
