//
//  DummyOptimizationPass.cpp
//  
//
//  Created by Jules Testard on 22/05/2014.
//
//
#include "llvm/Pass.h"
#include "Variable.h"
#include "StaticAnalysis.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include <string>

using namespace llvm;
using namespace std;

/**
 * BUGS FOUND AND FIXED :
 * 	-	Avoid static members for the passes. This generates a linking error when making the shared object.
 * 	-	raw_ostream& object does not like to be fed a std::endl symbol. Prefer to user "\n".
 *
 */
namespace {
  struct DummyOptimizationPass : public ModulePass {
    static char ID;
    StaticAnalysis * staticAnalysis;
    DummyOptimizationPass() : ModulePass(ID) {}

    virtual bool runOnModule(Module &M) {
    	//initialize analysis
    	staticAnalysis = new StaticAnalysis(M);

    	//Run worklist algorithm
    	staticAnalysis->runWorklist(M);
    	return false;
    }

    //The dummy optimization does not modify the code, but performs various analyses and outputs their result here
    void print(raw_ostream &OS, const Module*) const {

    	// ========== TESTING ==============
    	//Test that variable class is properly linked (test can be removed later).
    	string example = "example";
    	Variable v(example);
    	OS << "VARIABLE test : ";
    	OS << "I created a new variable with name : " << v.GetName() << "\n";

    	//The pure static analysis. Functional testing
    	OS << "STATIC ANALYSIS test : \n";
    	OS << "ID of root : " << staticAnalysis->getCFG().id << "\n";
    	OS << "Successors of CFG root : " << staticAnalysis->getCFG().succs.front().id << "\n";
    	OS << "Print CFG : " << staticAnalysis->JSONCFG() << "\n";
  	}
  };
}

char DummyOptimizationPass::ID = 0;
static RegisterPass<DummyOptimizationPass> X("dummyOptimization", "Dummy Optimization Pass", false, false);
