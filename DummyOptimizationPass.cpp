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

namespace {
  struct DummyOptimizationPass : public ModulePass {
    static char ID;
    static StaticAnalysis * staticAnalysis;
    DummyOptimizationPass() : ModulePass(ID) {}

    virtual bool runOnModule(Module &M) {
    	//initialize analysis
    	staticAnalysis = new StaticAnalysis();

    	//Run worklist algorithm
    	//staticAnalysis->runWorklist(M);
    	return false;
    }

    //The dummy optimization does not modify the code, but performs various analyses and outputs their result here
    void print(raw_ostream &OS, const Module*) const {
    	//Test that variable class is properly linked (test can be removed later).
    	string example = "example";
    	Variable v(example);
    	OS << "VARIABLE test : ";
    	OS << "I created a new variable with name : " << v.GetName() << "\n"; //Doesn't like std::endl or endl. Prefer to use "\n"

    	//The pure static analysis
    	OS << "STATIC ANALYSIS test : \n";
  	}
  };
}

char DummyOptimizationPass::ID = 0;
static RegisterPass<DummyOptimizationPass> X("dummyOptimization", "Dummy Optimization Pass", false, false);
