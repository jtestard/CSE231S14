//https://sites.google.com/site/arnamoyswebsite/Welcome/updates-news/llvmpasstoinsertexternalfunctioncalltothebitcode

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/IR/IRBuilder.h"
#include <map>
#include <string>

using namespace llvm;
using namespace std;

namespace {
	int beebee(int a)
	{
		a += 1;
		return a;
	}

  struct DynamicInstructionCount : public FunctionPass {
    static char ID;
    llvm::Function* hook;		//Required that you have a llvm Function to call
/*    map<string,int> instructionMap;
    llvm::IRBuilder<> &builder;
    llvm::Function* hook;		//Required that you have a llvm Function to call
    //Convert your regular function into a Constant type
    Constant* constFun;*/


    DynamicInstructionCount() : FunctionPass(ID) {hook = NULL;}

    virtual bool runOnModule(Module &M)
            {
                Constant *hookFunc;
                hookFunc = M.getOrInsertFunction("print", Type::getVoidTy(M.getContext()), (Type*)0);

                hook= cast<Function>(hookFunc);


                for(Module::iterator F = M.begin(), E = M.end(); F!= E; ++F)
                {

                    for(Function::iterator BB = F->begin(), E = F->end(); BB != E; ++BB)
                    {
                        DynamicInstructionCount::runOnBasicBlock(BB);
                    }
                }

                return false;
            }
            virtual bool runOnBasicBlock(Function::iterator &BB)
            {
                for(BasicBlock::iterator BI = BB->begin(), BE = BB->end(); BI != BE; ++BI)
                {
                        if(isa<LoadInst>(&(*BI)) )
                        {
                                LoadInst *CI = dyn_cast<LoadInst>(BI);
                                Instruction *newInst = CallInst::Create(hook, "");
                                BB->getInstList().insert((Instruction*)CI, newInst);
                        }

                }
                return true;
            }
        };
    }

    /*
    virtual bool doInitialization(Module & M) {

    	return false;
    }

    funfun = beebee;

    virtual bool runOnFunction(Function &F) {
    	for (inst_iterator I = inst_begin(F), E = inst_end(F) ; I != E ; ++I) {
        		string opcode = I->getOpcodeName();
        		I->insertAfter(builder.CreateCall(&funfun,NULL));
        		if (instructionMap[opcode]) {
        			instructionMap[opcode]++;
        		} else {
        			instructionMap[opcode] = 1;
        		}
        	}
    	return false;
    }

  	virtual bool doFinalization(Module &M) {
    	int total = 0;
    	for (map<string,int>::iterator it=instructionMap.begin() ; it!=instructionMap.end() ; it++) {
    		errs() << it->first << "\t\t" << it->second << "\n";
    		total += it->second;
    	}
    	errs() << "TOTAL\t\t" << total << "\n";
    	return false;
  	}
  };
}*/

char DynamicInstructionCount::ID = 0;
static RegisterPass<DynamicInstructionCount> X("dynamicInstructionCount", "DynamicInstructionCount World Pass", false, false);
