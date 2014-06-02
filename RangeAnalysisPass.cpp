//
//  DummyOptimizationPass.cpp
//  
//
//  Created by Jules Testard on 22/05/2014.
//
//
#include "llvm/Pass.h"
#include "Variable.h"
#include "RangeAnalysis.h"	//Subclasses StaticAnalysis.h and implements flow functons
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
  struct RangeAnalysisPass : public FunctionPass {
    static char ID;
    //vector<StaticAnalysis *>staticAnalyses;
    vector<RangeAnalysis*>rangeAnalyses;
    RangeAnalysisPass() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
    	//initialize analysis
    	//staticAnalyses.push_back(new StaticAnalysis(F));
    	rangeAnalyses.push_back(new RangeAnalysis(F));	//create a new worklist  and enqueue it to be run
    													//You will create a worklist per function in the source code
    	return false;
    }

    //The dummy optimization does not modify the code, but performs various analyses and outputs their result here
    void print(raw_ostream &OS, const Module*) const {

    	//The pure static analysis. Functional testing
    	OS << "Range analysis test : \n";
    	for (unsigned int i = 0 ; i < rangeAnalyses.size() ; i++){
    	        	OS << "Print CFG (without flow) : " << "\n";
    	        	//Check graph once. Everything flow should be empty.
    	        	rangeAnalyses[i]->JSONCFG(OS);
    	        	//Run worklist algorithm
    	        	rangeAnalyses[i]->runWorklist();
    	        	//Check graph again. Everything flow should say top.
    	        	OS << "\nPrint CFG (with flow) : " << "\n";
    	        	rangeAnalyses[i]->JSONCFG(OS);
/*    	for (unsigned int i = 0 ; i < staticAnalyses.size() ; i++){
        	OS << "Print CFG (without flow) : " << "\n";
        	//Check graph once. Everything flow should be empty.
        	staticAnalyses[i]->JSONCFG(OS);
        	//Run worklist algorithm
        	staticAnalyses[i]->runWorklist();
        	//Check graph again. Everything flow should say top.
        	OS << "\nPrint CFG (with flow) : " << "\n";
        	staticAnalyses[i]->JSONCFG(OS);*/
    	}

  	}
  };
}

char RangeAnalysisPass::ID = 0;
static RegisterPass<RangeAnalysisPass> X("rangeAnalysis", "Range analysis Pass", false, false);
