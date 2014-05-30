//
//  PointerAnalysisPass.cpp
//  
//
//  Created by Jules Testard on 22/05/2014.
//
//
#include "llvm/Pass.h"
#include "Variable.h"
#include "PointerAnalysis.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include <string>
#include <vector>

using namespace llvm;
using namespace std;

/**
 * BUGS FOUND AND FIXED :
 * 	-	Avoid static members for the passes. This generates a linking error when making the shared object.
 * 	-	raw_ostream& object does not like to be fed a std::endl symbol. Prefer to user "\n".
 *
 */
namespace {
  struct PointerAnalysisPass : public FunctionPass {
    static char ID;
    vector<PointerAnalysis *>PointerAnalyses;
    PointerAnalysisPass() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
    	//initialize analysis
    	PointerAnalyses.push_back(new PointerAnalysis(F));
    	return false;
    }

    //The dummy optimization does not modify the code, but performs various analyses and outputs their result here
    void print(raw_ostream &OS, const Module*) const {

    	//Has not been tested. In the works.
    	OS << "POINTER ANALYSES test : \n";
    	for (unsigned int i = 0 ; i < PointerAnalyses.size() ; i++){
        	OS << "Function Name : " << PointerAnalyses[i]->getFunctionName() << "\n";
        	OS << "First Instruction Name : " << PointerAnalyses[i]->getCFG()->succs[0]->inst->getName().str() << "\n";
        	OS << "Print CFG : " << "\n";

        	//Check graph once. Everything flow should be empty.
        	PointerAnalyses[i]->JSONCFG(OS);
        	//Run worklist algorithm
        	PointerAnalyses.back()->runWorklist();
        	//Check graph again. Everything flow should contain appropriate flow.
        	PointerAnalyses[i]->JSONCFG(OS);
    	}

  	}
  };
}

char PointerAnalysisPass::ID = 0;
static RegisterPass<PointerAnalysisPass> X("pointerAnalysisOptimization", "Pointer Analysis Optimization Pass", false, false);
