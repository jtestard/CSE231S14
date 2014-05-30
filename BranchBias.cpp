#include "llvm/Pass.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/IRBuilder.h"
#include <iostream>
#define HELPER_FUNCTIONS 0

using namespace std;

using namespace llvm;

namespace {
    struct BranchBias : public ModulePass {
        static char ID;

        BranchBias() : ModulePass (ID) {}

        virtual bool runOnModule (Module &M) {
            // These are the helper functions for this part
      	    Function *bFound; //count the number of branches
            Function *bTaken; // count the taken branches
            Function *printStats; // Print method of the helper. Print stats.
            StringRef functionName; // Name of the function that gets analyzed currently. We'll need to print this name.

            // function ptrs. We have a pass called DumpContent.cpp that prints the name of the functions
            // so we used it to figure out the appropriate function names of the helper
            bTaken = cast<Function> (M.getOrInsertFunction("_Z11branchTakenPc",
                                          Type::getVoidTy(M.getContext()), 
                                          Type::getInt8PtrTy(M.getContext()), 
                                          (Type*)0));
            bFound = cast<Function> (M.getOrInsertFunction("_Z11branchFoundPc",
                                          Type::getVoidTy(M.getContext()), 
                                          Type::getInt8PtrTy(M.getContext()), 
                                          (Type*)0));
            printStats = cast<Function> (M.getOrInsertFunction("_Z5printv",
                                            Type::getVoidTy(M.getContext()), 
                                            (Type*)0));

            Module::iterator F = M.begin();
            // We initially run this pass on the linked .bc so we had to iterate over all the
            // functions of the helper (160 in total), we don't really need to do this any more since we no longer
            // use the linked bc...
			//for (int i = 0 ; i < HELPER_FUNCTIONS ; i++)
			//	F++;

            for(Module::iterator E = M.end(); F!= E; ++F)
            {
                // name of curr function
                functionName = (*F).getName();
                // let's go through all the basic blocks.
                for(Function::iterator BB = F->begin(), E = F->end(); BB != E; ++BB) {
                    BranchBias::runOnBasicBlock(BB, bFound, bTaken, functionName);
                }

                // Let's add a call to the print functions to see the stats.
                // We have to do it before the return statement.
                if ((*F).getName() == "main") {
                    Function::iterator lastBlock = F->end();
                    lastBlock--;
                    BranchBias::addPrintCall(lastBlock, printStats);
                }
            }
            return false;
        }
        // Run this to spot branches
        virtual bool runOnBasicBlock (Function::iterator &BB, Function * bFound, Function * bTaken, StringRef functionName) {
            
            Value *myStr;
            bool isFirst = true;

            for(BasicBlock::iterator BI = BB->begin(), BE = BB->end(); BI != BE; ++BI)
            {
                //Found a branch instruction
                if(isa<BranchInst>(&(*BI)) ) {
                    BranchInst *CI = dyn_cast<BranchInst>(BI);
                    // check if it's an unconditional branch.
                    if (CI->isUnconditional()) {
                        continue;
                    }

                    // It's either Taken or Not.
                    if (CI->getNumSuccessors() != 2) {
                        continue;
                    }

                    // getSuccessor(0) is the "taken" basic block. Perform the instrumentation
                    // add a call to the function that increases the "taken" counter.
                    BasicBlock *block = CI->getSuccessor(0);
                    BasicBlock::iterator takenInsertPt = block->getFirstInsertionPt();


                    // The insert point for this counter is the first command of the
                    // taken branch...
                    IRBuilder<> builder(block);
                    builder.SetInsertPoint(takenInsertPt);


                    if (isFirst) {
                        myStr = builder.CreateGlobalStringPtr(functionName, "myStr");
                        isFirst = false;
                    }
                    builder.CreateCall (bTaken, myStr);



                    // We found a conditional branch, let's call the appropriate function
                    // to increase the counter of the "found" branches
                    builder.SetInsertPoint(CI);
                    builder.CreateCall (bFound, myStr);


                }
            }
            return true;
        }

        virtual void addPrintCall (Function::iterator BB, Function* printStats) {
            // Create an IRBuilder instance and set the insert point 
            // to the very last instruction of main (before return)
            IRBuilder<> builder(BB);
            BasicBlock::iterator lastIns = BB->end();
            lastIns--;
            builder.SetInsertPoint(lastIns);

            // call function that prints results...
            builder.CreateCall (printStats, "");
        }
    };
}

char BranchBias::ID = 0;
static RegisterPass<BranchBias> X("branchbias", "Branch Bias", false, false);
