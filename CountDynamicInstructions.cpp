//https://sites.google.com/site/arnamoyswebsite/Welcome/updates-news/llvmpasstoinsertexternalfunctioncalltothebitcode
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#define HELPER_FUNCTIONS 91
#define COUNT_FUNCTION_IDX 4

using namespace llvm;
using namespace std;

namespace {
  struct DynamicInstructionCount : public ModulePass {
    static char ID;
    llvm::Function* hookCount;		//Required that you have a llvm Function to call
    llvm::Function* hookPrint;

    DynamicInstructionCount() : ModulePass(ID) {hookCount = NULL; hookPrint = NULL;}

    virtual bool runOnModule(Module &M){
                Constant *hookFunc;
                StringRef countingFunctionName;
                StringRef printFunctionName;
                hookFunc = M.getOrInsertFunction("counting",Type::getVoidTy(M.getContext()),Type::getInt8PtrTy(M.getContext()),(Type*)0);
                if (hookFunc) {
					hookCount = cast<Function>(hookFunc);
					Module::iterator F = M.begin();
					for (int i = 0 ; i < HELPER_FUNCTIONS ; i++)
						F++;
					for(Module::iterator E = M.end(); F!= E; ++F) {
						errs() <<  F->getName() << "\n";
						if (!F->getName().equals(printFunctionName) && !F->getName().equals(countingFunctionName))
							for(Function::iterator BB = F->begin(), E = F->end(); BB != E; ++BB) {
								DynamicInstructionCount::runOnBasicBlock(BB);
							}
					}
                }
                hookFunc = M.getOrInsertFunction("print",Type::getVoidTy(M.getContext()),(Type*)0);
                if (hookFunc) {
                	hookPrint = cast<Function>(hookFunc);
                	Module::iterator F = M.end();
                	F--;
                	Function::iterator BB = F->end();
                	BB--;
                	BasicBlock::iterator BI = BB->end();
                	BI--;
                	Instruction *CI = dyn_cast<Instruction>(BI);
                	Instruction *newInst = CallInst::Create(hookPrint,"");
                	BB->getInstList().insert(CI, newInst);
                }

                return false;
            }

    virtual bool runOnBasicBlock(Function::iterator &BB) {
                IRBuilder<> builder(BB);
                string result = "";
    			for(BasicBlock::iterator BI = BB->begin(), BE = BB->end(); BI != BE; ++BI) {
                	Instruction *CI = dyn_cast<Instruction>(BI);
                	const char * opcodeName = CI->getOpcodeName();
                	//Value* argument = ConstantDataArray::getString(context,StringRef(opcodeName));
//                	result.append(opcodeName);
//                	result.append("|");
                	//Value* argument = ConstantFP::get(Type::getInt32Ty(context),StringRef(opcodeName));
                	//Value * argument = ConstantInt::get(Type::getInt32Ty(context),1);
//                	vector<Value*> args = vector<Value*>();
//                	args.push_back(argument);
//                	ArrayRef<Value*> Args(args);
//                	argument->dump();
//                	FunctionType *FTy = cast<FunctionType>(cast<PointerType>(hookCount->getType())->getElementType());
//                	errs() << Args.size() << "\t" << FTy->getNumParams() << "\n";
//                	for (unsigned i = 0; i != Args.size(); ++i) {
//                		errs() << *FTy->getParamType(i) << "\t";
//                		errs() << *Args[i]->getType() << "\n";
//                	}
                	//argument->dump();
                	//Instruction *newInst = CallInst::Create(hookCount,args);
                	//Instruction *newInst = builder.CreateCall(hookCount,argument);
                	//BB->getInstList().insert(CI, newInst);
                }
//    			BasicBlock::iterator BI = BB->end();
//    			BI--;
//            	builder.SetInsertPoint(BB,BI);
//            	Value* argument = builder.CreateGlobalStringPtr(StringRef(result),"myStr");
//            	argument->dump();
//            	builder.CreateCall(hookCount,argument);
                return true;
    }
  };
}

char DynamicInstructionCount::ID = 0;
static RegisterPass<DynamicInstructionCount> X("dynamicInstructionCount", "DynamicInstructionCount", false, false);
