//
//  DummyOptimizationPass.cpp
//  
//
//  Created by Jules Testard on 22/05/2014.
//
//
#include "llvm/Pass.h"
#include "Variable.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include <string>
#include <stdio.h>

using namespace llvm;
using namespace std;

namespace {
  struct DummyOptimizationPass : public ModulePass {
    static char ID;
    DummyOptimizationPass() : ModulePass(ID) {}

    virtual bool runOnModule(Module &M) {
    	//printf("MIMO \n");
    	return false;
    }

    //CAlled on the analyze flag (does not transform the code I beleive)
    void print(raw_ostream &OS, const Module*) const {
    	string example = "example";
    	Variable v(example);
    	OS << "VARIABLE test : ";
    	OS << "I created a new variable with name : " << v.GetName() << "\n"; //Doesn't like std::endl or endl. Prefer to use "\n"
  	}
  };
}

char DummyOptimizationPass::ID = 0;
static RegisterPass<DummyOptimizationPass> X("constProp", "Constant propagation Pass", false, false);
