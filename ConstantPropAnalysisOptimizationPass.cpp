
//
//  ConstantPropAnalysisOptimizationPass.cpp
//  
//
//  Created by Costas Zarifis on 22/05/2014.
//
//
#include "llvm/Pass.h"
#include "Variable.h"
#include "ConstantPropAnalysis.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include <string>
#include <vector>

using namespace llvm;
using namespace std;

/*
 * BUGS FOUND AND FIXED :
 * 	-	Avoid static members for the passes. This generates a linking error when making the shared object.
 * 	-	raw_ostream& object does not like to be fed a std::endl symbol. Prefer to user "\n".
 */

namespace {
  struct ConstantPropAnalysisOptimizationPass : public FunctionPass {
    static char ID;
    vector<ConstantPropAnalysis *>constantPropAnalyses;
    ConstantPropAnalysisOptimizationPass() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
    	//initialize analysis
    	constantPropAnalyses.push_back(new ConstantPropAnalysis(F));
    	return false;
    }

    //The dummy optimization does not modify the code, but performs various analyses and outputs their result here
    void print(raw_ostream &OS, const Module*) const {

    	//The pure static analysis. Functional testing
    	for (unsigned int i = 0 ; i < constantPropAnalyses.size() ; i++){
        	//Run worklist algorithm
        	constantPropAnalyses[i]->runWorklist();
        	//Check analysis results.
        	OS << "\nPrint CFG (with flow) : " << "\n";
        	constantPropAnalyses[i]->JSONCFG(OS);
    	}

  	}
  };
}

char ConstantPropAnalysisOptimizationPass::ID = 0;
static RegisterPass<ConstantPropAnalysisOptimizationPass> X("ConstantPropAnalysisOptimization", "Constant Propagation Analysis Optimization Pass", false, false);
