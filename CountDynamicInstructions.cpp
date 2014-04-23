//https://sites.google.com/site/arnamoyswebsite/Welcome/updates-news/llvmpasstoinsertexternalfunctioncalltothebitcode
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"

using namespace llvm;
using namespace std;

namespace {
  struct DynamicInstructionCount : public ModulePass {
    static char ID;
    llvm::Function* hook;		//Required that you have a llvm Function to call

    DynamicInstructionCount() : ModulePass(ID) {hook = NULL;}

    virtual bool runOnModule(Module &M){
                Constant *hookFunc;
                StringRef countingFunctionName;
                StringRef printFunctionName;

                //Function names follow linking order and order of functions in file.
                Module::iterator Fit = M.begin();
                countingFunctionName = Fit->getName(); //get name for counting function from helper.cpp
                Fit++;
                printFunctionName = Fit->getName(); //get name for print function from helper.cpp

                for (Module::iterator F = M.begin(), E = M.end(); F!= E; ++F) {
                	errs() << "Function : " << F->getName() << "\n";
                }

                hookFunc = M.getFunction(printFunctionName);
                if (hookFunc) {
					hook= cast<Function>(hookFunc);
					for(Module::iterator F = M.begin(), E = M.end(); F!= E; ++F) {
						if (!F->getName().equals(printFunctionName) && !F->getName().equals(countingFunctionName))
							for(Function::iterator BB = F->begin(), E = F->end(); BB != E; ++BB) {
								DynamicInstructionCount::runOnBasicBlock(BB);
							}
					}
                }
                return false;
            }

    virtual bool runOnBasicBlock(Function::iterator &BB) {
                for(BasicBlock::iterator BI = BB->begin(), BE = BB->end(); BI != BE; ++BI) {
                	Instruction *CI = dyn_cast<Instruction>(BI);
                	Instruction *newInst = CallInst::Create(hook, "");
                	BB->getInstList().insert(CI, newInst);
                }
                return true;
    }
  };
}

char DynamicInstructionCount::ID = 0;
static RegisterPass<DynamicInstructionCount> X("dynamicInstructionCount", "DynamicInstructionCount World Pass", false, false);
