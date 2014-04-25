//https://sites.google.com/site/arnamoyswebsite/Welcome/updates-news/llvmpasstoinsertexternalfunctioncalltothebitcode
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#define HELPER_FUNCTIONS 160

using namespace llvm;
using namespace std;

namespace {
  struct DynamicInstructionCount : public ModulePass {
    static char ID;
    llvm::Function* hookCount;		//Required that you have a llvm Function to call
    llvm::Function* hookPrint;

    DynamicInstructionCount() : ModulePass(ID) {hookCount = NULL; hookPrint = NULL;}

    virtual bool runOnModule(Module &M){
		// Mangled names of functions
		StringRef printFunctionName = "_Z5printv";
		StringRef countingFunctionName = "_Z8countingPc";
		StringRef mainName = "main";

		//Hook functions with types matching with functions from helper.cpp
		hookCount = cast<Function> (M.getOrInsertFunction(countingFunctionName,Type::getVoidTy(M.getContext()),Type::getInt8PtrTy(M.getContext()),(Type*)0));
		hookPrint = cast<Function> (M.getOrInsertFunction(printFunctionName,Type::getVoidTy(M.getContext()),(Type*)0));
		hookCount->setCallingConv(CallingConv::C);

		//Do not analyze helper functions (skip them in iteration). The number of helper functions depends on helper.cpp and included librairies.
		Module::iterator F = M.begin();
		for (int i = 0 ; i < HELPER_FUNCTIONS ; i++)
			F++;

		for(Module::iterator E = M.end(); F!= E; ++F) {
			if (!F->getName().equals(printFunctionName) && !F->getName().equals(countingFunctionName))
				for(Function::iterator BB = F->begin(), E = F->end(); BB != E; ++BB) {
					DynamicInstructionCount::runOnBasicBlock(BB);
				}
				// Print function is hooked to the last block of the main function
				if (F->getName().equals(mainName)) {
					Function::iterator BB = F->end();
					if (BB!=F->begin())
						BB--;
					BasicBlock::iterator BI = BB->end();
					if (BI != BB->begin())
						BI--;
					Instruction *CI = dyn_cast<Instruction>(BI);
					Instruction *newInst = CallInst::Create(hookPrint, "");
					BB->getInstList().insert(CI, newInst);
				}
		}
		return false;
    }
    // Hook the counting function on each basic block
    virtual bool runOnBasicBlock(Function::iterator &BB) {
		IRBuilder<> builder(BB);
		string result = "";
		for(BasicBlock::iterator BI = BB->begin(), BE = BB->end(); BI != BE; ++BI) {
			Instruction *CI = dyn_cast<Instruction>(BI);
			result.append(CI->getOpcodeName());
			result.append(" ");
		}
		BasicBlock::iterator BE = BB->end();
		if (BE != BB->begin())
			BE--;
		builder.SetInsertPoint(BB,BE);
		Value* myStr = builder.CreateGlobalStringPtr(result,"myStr");
		builder.CreateCall(hookCount,myStr);
		return true;
    }
  };
}

char DynamicInstructionCount::ID = 0;
static RegisterPass<DynamicInstructionCount> X("dynamicInstructionCount", "DynamicInstructionCount", false, false);

