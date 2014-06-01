//
//  PointerAnalysisOptimizationPass.cpp
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
  struct PointerAnalysisOptimizationPass : public FunctionPass {
    static char ID;
    vector<PointerAnalysis *>pointerAnalyses;
    PointerAnalysisOptimizationPass() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
    	//initialize analysis
    	pointerAnalyses.push_back(new PointerAnalysis(F));
    	return false;
    }

    //The dummy optimization does not modify the code, but performs various analyses and outputs their result here
    void print(raw_ostream &OS, const Module*) const {

    	//The pure static analysis. Functional testing
    	OS << "POINTER ANALYSES test : \n";
    	for (unsigned int i = 0 ; i < pointerAnalyses.size() ; i++){
        	OS << "Print CFG (without flow) : " << "\n";
        	//Check graph once. Everything flow should be empty.
        	//pointerAnalyses[i]->JSONCFG(OS);
        	//Run worklist algorithm
        	pointerAnalyses[i]->runWorklist();
        	//Check graph again. Everything flow should say top.
        	OS << "\nPrint CFG (with flow) : " << "\n";
        	//pointerAnalyses[i]->JSONCFG(OS);
    	}

  	}
  };
}

char PointerAnalysisOptimizationPass::ID = 0;
static RegisterPass<PointerAnalysisOptimizationPass> X("pointerAnalysisOptimization", "Pointer Analysis Optimization Pass", false, false);
